#include "physics/de_particle.hpp"
#include "physics/de_geometry.hpp"

DeParticle::DeParticle() {
  type = RIGIDBODY_TYPE_PARTICLE;
  friction = 0.95f;
  bounce = 0.7f;
  gravity = glm::vec3(0.0f, -9.81f, 0.0f);

  #ifdef EULER_INTEGRATION
    mass;
  #endif
}

void DeParticle::Update(float dt) {
  #ifdef EULER_INTEGRATION
    oldPosition = position;
    glm::vec3 acceleration = forces * InvMass();

    #ifdef ACCURATE_EULER_INTEGRATION
      glm::vec3 oldVelocity = velocity;
      velocity = velocity * friction + acceleration * dt;
      position = position + (oldVelocity + velocity) * 0.5f * dt;
    #else
      velocity = velocity * friction + acceleration * dt;
      position = position + velocity * dt;
    #endif
  
  #else
    vec3 velocity = position - oldPosition;
    oldPosition = position;
    float deltaSquare = dt * dt;
    position = position + (velocity * friction + forces * deltaSquare);
  #endif
}

void DeParticle::ApplyForces() {
  #ifdef EULER_INTEGRATION
    forces = gravity * mass;
  #else
    forces = gravity * mass;
  #endif
}

void DeParticle::SolveConstraints(const std::vector<OBB>& constraints) {
  int size = constraints.size();
  for (int i = 0; i < size; ++i) {
    Line traveled(oldPosition, position);
    if (Linetest(constraints[i], traveled)) {
      // if (PointInOBB(position, constraints[i])) {
      #ifndef EULER_INTEGRATION
        glm::vec3 velocity = position - oldPosition;
      #endif
      glm::vec3 direction = glm::normalize(velocity);
      Ray ray(oldPosition, direction);
      RaycastResult result;

      if (Raycast(constraints[i], ray, &result)) {
        // Place object just a little above collision result
        position = result.point + result.normal * 0.003f;

        glm::vec3 vn = result.normal * glm::dot(result.normal, velocity);
        glm::vec3 vt = velocity - vn;

        #ifdef EULER_INTEGRATION
          oldPosition = position;
          velocity = vt - vn * bounce;
        #else
          oldPosition = position - (vt - vn * bounce);
        #endif

        break;
      }
    }
  }
}

void DeParticle::SetPosition(const glm::vec3& pos) {
  position = pos;
  oldPosition = pos;
}

glm::vec3 DeParticle::GetPosition() {
  return position;
}

void DeParticle::SetBounce(float b) {
  bounce = b;
}

float DeParticle::GetBounce() {
  return bounce;
}

void DeParticle::AddImpulse(const glm::vec3& impulse) {
  #ifdef EULER_INTEGRATION
    velocity = velocity + impulse;
  #else
    glm::vec3 velocity = position - oldPosition;
    velocity = velocity + impulse;
    oldPosition = position - velocity;
  #endif
}

float DeParticle::InvMass() {
  if (mass == 0.0f) return 0.0f;
  return 1.0f / mass;
}

void DeParticle::SetMass(float m) {
  mass = m;
}

glm::vec3 DeParticle::GetVelocity() {
  #ifdef EULER_INTEGRATION
    return velocity;
  #else
    return positino - oldPosition;
  #endif
}

void DeParticle::SetFriction(float f) {
  friction = f;
}
