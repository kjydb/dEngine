#include <iostream>

#include "physics/de_physicsSystem.hpp"
#include "physics/de_rigidbodyVolume.hpp"

DePhysicsSystem::DePhysicsSystem() {
  LinearProjectionPercent = 0.45f;
  PenetrationSlack = 0.01f;
  ImpulseIteration = 5;

  DebugRender = false;
  DoLinearProjection = true;
  RenderRandomColors = false;

  colliders1.reserve(100);
  colliders2.reserve(100);
  results.reserve(100);
}

void DePhysicsSystem::Update(float dt) {
  colliders1.clear();
  colliders2.clear();
  results.clear();

  // Find objects whom are colliding.
  // First, build a list of colliding objects.
  {
    CollisionManifold result;
    for (int i = 0, size = bodies.size(); i < size; ++i) {
      for (int j = i; j < size; ++j) {
        if (i == j) continue;

        ResetCollisionManifold(&result);
        if (bodies[i]->HasVolume() && bodies[j]->HasVolume()) {
          DeRigidbodyVolume* m1 = (DeRigidbodyVolume*)bodies[i];
          DeRigidbodyVolume* m2 = (DeRigidbodyVolume*)bodies[j];
          result = FindCollisionFeatures(*m1, *m2);
        }

        if (result.colliding) {
          colliders1.push_back(bodies[i]);
          colliders2.push_back(bodies[j]);
          results.push_back(result);
        }
      }
    }
  }

  // Calculate forces acting on the object.
  for (int i = 0, size = bodies.size(); i < size; ++i) {
    bodies[i]->ApplyForces();
  }

  // Calculate forces acting on cloths.
  // Skip this for now.
  // for (int i = 0, size = cloths.size(); i < size; ++i) {
  //   cloths[i]->ApplyForces();
  // }

  // Apply impulses to resolve collisions.
  for (int k = 0; k < ImpulseIteration; ++k) {
    for (int i = 0, iSize = results.size(); i < iSize; ++i) {
      for (int j = 0, jSize = results[i].contacts.size(); j < jSize; ++j) {
        DeRigidbodyVolume* m1 = (DeRigidbodyVolume*)colliders1[i];
        DeRigidbodyVolume* m2 = (DeRigidbodyVolume*)colliders2[i];
        ApplyImpulse(*m1, *m2, results[i], j);
      }
    }
  }

  // Integrate velocity and impulse of objects.
  for (int i = 0, size = bodies.size(); i < size; ++i) {
    bodies[i]->Update(dt);
  }

  // Integrate velocity and impulse of cloths.
  // Skip for now.
  // for (int i = 0, size = cloths.size(); i < size; +i) {
  //   cloths[i]->Update(dt);
  // }

  // Correct position to avoid sinking.
  if (DoLinearProjection) {
    for (int i = 0, size = results.size(); i < size; ++i) {
      if (!colliders1[i]->HasVolume() && !colliders2[i]->HasVolume()) continue;

      DeRigidbodyVolume* m1 = (DeRigidbodyVolume*)colliders1[i];
      DeRigidbodyVolume* m2 = (DeRigidbodyVolume*)colliders2[i];
      float totalMass = m1->InvMass() + m2->InvMass();

      if (totalMass == 0.0f) continue;

      float depth = fmaxf(results[i].depth - PenetrationSlack, 0.0f);
      float scalar = (totalMass == 0.0f) ? 0.0f : depth / totalMass;
      glm::vec3 correction = results[i].normal * scalar * LinearProjectionPercent;

      m1->position = m1->position - correction * m1->InvMass();
      m2->position = m2->position + correction * m2->InvMass();

      m1->SyncCollisionVolumes();
      m2->SyncCollisionVolumes();
    }
  }

  // // Apply spring forces
  // for (int i = 0, size = springs.size(); i < size; ++i) {
  //   springs[i].ApplyForce(dt);
  // }

  // // Apply spring forces for cloths
  // for (int i = 0, size = cloths.size(); i < size; ++i) {
  //   cloths[i]->ApplySpringForces(dt);
  // }

  // Solve constraints
  for (int i = 0, size = bodies.size(); i < size; ++i) {
    bodies[i]->SolveConstraints(constraints);
  }

  // // Solve constraints for cloths
  // for (int i = 0, size = cloths.size(); i < size; ++i) {
  //   cloths[i]->SolveConstraints(constraints);
  // }
}

void DePhysicsSystem::AddRigidbody(DeRigidbody* body) {
  bodies.push_back(body);
}

void DePhysicsSystem::AddCloth(DeCloth* cloth) {
  cloths.push_back(cloth);
}

void DePhysicsSystem::AddSpring(const DeSpring& spring) {
  springs.push_back(spring);
}

void DePhysicsSystem::AddConstraint(const OBB& constraint) {
  constraints.push_back(constraint);
}

void DePhysicsSystem::ClearRigidbodies() {
  bodies.clear();
}

void DePhysicsSystem::ClearCloths() {
  cloths.clear();
}

void DePhysicsSystem::ClearSprings() {
  springs.clear();
}

void DePhysicsSystem::ClearConstraints() {
  constraints.clear();
}
