#include <iostream>

#include "physics/de_rigidbodyVolume.hpp"
#include "utils/math_util.hpp"

void DeRigidbodyVolume::ApplyForces() {
  forces = GRAVITY_CONST * mass;
}

void DeRigidbodyVolume::SyncCollisionVolumes() {
  sphere.position = position;
  box.position = position;

  box.orientation = Rotation3x3(glm::radians(orientation.x), glm::radians(orientation.y), glm::radians(orientation.z));
}

void DeRigidbodyVolume::Update(float dt) {
  // Integrate velocity
  const float damping = 0.98f;

  glm::vec3 acceleration = forces * InvMass();
  velocity = velocity + acceleration * dt;
  velocity = velocity * damping;

  if (fabsf(velocity.x) < 0.001f) velocity.x = 0.0f;
  if (fabsf(velocity.y) < 0.001f) velocity.y = 0.0f;
  if (fabsf(velocity.z) < 0.001f) velocity.z = 0.0f;

  if (type == RIGIDBODY_TYPE_BOX) {
    glm::vec3 angAccel = MultiplyVector(torques, InvTensor());
    angVel = angVel + angAccel * dt;
    angVel = angVel * damping;

    if (fabsf(angVel.x) < 0.001f) angVel.x = 0.0f;
    if (fabsf(angVel.y) < 0.001f) angVel.y = 0.0f;
    if (fabsf(angVel.z) < 0.001f) angVel.z = 0.0f;
  }

  // Integrate position
  position = position + velocity * dt;
  
  if (type == RIGIDBODY_TYPE_BOX) {
    orientation = orientation + angVel * dt;
  }

  SyncCollisionVolumes();
}

glm::mat4 DeRigidbodyVolume::InvTensor() {
  if (mass == 0) return glm::mat4(0.0f);
  
  float ix = 0.0f;
  float iy = 0.0f;
  float iz = 0.0f;
  float iw = 0.0f;

  if (mass != 0 && type == RIGIDBODY_TYPE_SPHERE) {
    float r2 = sphere.radius * sphere.radius;
    float fraction = (2.0f / 5.0f);

    ix = r2 * mass * fraction;
    iy = r2 * mass * fraction;
    iz = r2 * mass * fraction;
    iw = 1.0f;
  }
  else if (mass != 0 && type == RIGIDBODY_TYPE_BOX) {
    glm::vec3 size = box.size * 2.0f;
    float fraction = (1.0f / 12.0f);

    float x2 = size.x * size.x;
    float y2 = size.y * size.y;
    float z2 = size.z * size.z;

    ix = (y2 + z2) * mass * fraction;
    iy = (x2 + z2) * mass * fraction;
    iz = (x2 + y2) * mass * fraction;
    iw = 1.0f;
  }

	// printf("InvTensor(): ( %f, %f, %f, %f )\n", ix, iy, iz, iw);

  return glm::inverse(glm::mat4(
    ix, 0.0f, 0.0f, 0.0f,
    0.0f, iy, 0.0f, 0.0f,
    0.0f, 0.0f, iz, 0.0f,
    0.0f, 0.0f, 0.0f, iw
  ));
}

void DeRigidbodyVolume::AddLinearImpulse(const glm::vec3& impulse) {
  printf("AddLinearImpulse implementation needed.");
}

void DeRigidbodyVolume::AddRotationalImpulse(const glm::vec3& point, const glm::vec3& impulse) {
  printf("AddRotationalImpulse implementation needed.");
}

CollisionManifold FindCollisionFeatures(DeRigidbodyVolume& ra, DeRigidbodyVolume& rb) {
  CollisionManifold result;
  ResetCollisionManifold(&result);

  if (ra.type == RIGIDBODY_TYPE_SPHERE) {
    if (rb.type == RIGIDBODY_TYPE_SPHERE) {
      result = FindCollisionFeatures(ra.sphere, rb.sphere);
    } 
    else if (rb.type == RIGIDBODY_TYPE_BOX) {
      result = FindCollisionFeatures(rb.box, ra.sphere);
      result.normal *= -1.0f;
    }
  }
  else if (ra.type == RIGIDBODY_TYPE_BOX) {
    if (rb.type == RIGIDBODY_TYPE_BOX) {
      result = FindCollisionFeatures(ra.box, rb.box);
    }
    else if (rb.type == RIGIDBODY_TYPE_SPHERE) {
      result = FindCollisionFeatures(ra.box, rb.sphere);
    }
  }

  return result;
}

void ApplyImpulse(DeRigidbodyVolume& A, DeRigidbodyVolume& B, const CollisionManifold& M, int c) {
  float invMass1 = A.InvMass();
  float invMass2 = B.InvMass();
  float invMassSum = invMass1 + invMass2;

  if (invMassSum == 0.0f) return;  // Both objects have infinate mass.

  glm::vec3 r1 = M.contacts[c] - A.position;
  glm::vec3 r2 = M.contacts[c] - B.position;
  glm::mat4 i1 = A.InvTensor();
  glm::mat4 i2 = B.InvTensor();

  glm::vec3 relativeVel = (B.velocity + glm::cross(B.angVel, r2)) - (A.velocity + glm::cross(A.angVel, r1));

  // relative collision normal
  glm::vec3 relativeNorm = M.normal;
  relativeNorm = glm::normalize(relativeNorm);

  // if moving away from each other, do nothing.
  if (glm::dot(relativeVel, relativeNorm) > 0.0f) return;

  float e = fminf(A.cor, B.cor);

  float numerator = (-(1.0f + e) * glm::dot(relativeVel, relativeNorm));
  float d1 = invMassSum;

  glm::vec3 d2 = glm::cross(MultiplyVector(glm::cross(r1, relativeNorm), i1), r1);  // TODO: Check if MultiplyVector() is correct.
  glm::vec3 d3 = glm::cross(MultiplyVector(glm::cross(r2, relativeNorm), i2), r2);
  float denominator = d1 + glm::dot(relativeNorm, d2 + d3);

  float j = (denominator == 0.0f) ? 0.0f : numerator / denominator;
  if (M.contacts.size() > 0.0f && j != 0.0f) {
    j /= (float)M.contacts.size();
  }

  glm::vec3 impulse = relativeNorm * j;
  A.velocity = A.velocity - impulse * invMass1;
  B.velocity = B.velocity + impulse * invMass2;


  A.angVel = A.angVel - MultiplyVector(glm::cross(r1, impulse), i1);
  B.angVel = B.angVel + MultiplyVector(glm::cross(r2, impulse), i2);

  // Friction
  glm::vec3 t = relativeVel - (relativeNorm * glm::dot(relativeVel, relativeNorm));
  if (CMP(glm::dot(t, t), 0.0f)) {
    return;
  }
  t = glm::normalize(t);

  numerator = -glm::dot(relativeVel, t);
  d1 = invMassSum;
  d2 = glm::cross(MultiplyVector(glm::cross(r1, t), i1), r1);
  d3 = glm::cross(MultiplyVector(glm::cross(r2, t), i2), r2);
  denominator = d1 + glm::dot(t, d2 + d3);

  float jt = (denominator == 0.0f) ? 0.0f : numerator / denominator;
  if (M.contacts.size() > 0.0f && jt != 0.0f) {
    jt /= (float)M.contacts.size();
  }

  if (CMP(jt, 0.0f)) return;

  glm::vec3 tangentImpulse;
  #ifdef DYNAMIC_FRICTION
    float sf = sqrtf(A.staticFriction * B.staticFriction);
    float df = sqrtf(A.dynamicFriction * B.dynamicFriction);
    if (fabsf(jt) < j * sf) {
      tangetnImpulse = t * jt;
    }
    else {
      tangentImpulse = t * -j * df;
    }
  #else
    float friction = sqrtf(A.friction * B.friction);
    if (jt > j * friction) {
      jt = j * friction;
    }
    else if (jt < -j * friction) {
      jt = -j * friction;
    }
    tangentImpulse = t * jt;
  #endif

  A.velocity = A.velocity - tangentImpulse * invMass1;
  B.velocity = B.velocity + tangentImpulse * invMass2;

  A.angVel = A.angVel - MultiplyVector(glm::cross(r1, tangentImpulse), i1);
  B.angVel = B.angVel + MultiplyVector(glm::cross(r2, tangentImpulse), i2);
}
