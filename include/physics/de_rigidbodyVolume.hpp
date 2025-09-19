#pragma once

#include "physics/de_geometry.hpp"
#include "physics/de_rigidbody.hpp"

#define GRAVITY_CONST glm::vec3(0.0f, -9.81f, 0.0f)

class DeRigidbodyVolume : public DeRigidbody {
  public:
    inline DeRigidbodyVolume() :
      cor(0.5f), mass(1.0f),
      #ifdef DYNAMIC_FRICTION
        staticFriction(0.5f),
        dynamicFriction(0.5f),
      #else
        friction(0.6f)
      #endif
      { type = RIGIDBODY_TYPE_BASE; }
    
    inline DeRigidbodyVolume(int bodyType) :
      cor(0.5f), mass(1.0f),
      #ifdef DYNAMIC_FRICTION
        staticFriction(0.5f),
        dynamicFriction(0.5f),
      #else
        friction(0.6f)
      #endif
      { type = bodyType; }
    
    virtual ~DeRigidbodyVolume() { }

    virtual void ApplyForces();
    void SyncCollisionVolumes();

    virtual void Update(float dt);
    // virtual void Render();

    inline float InvMass() { return mass == 0.0f ? 0.0f : 1.0f / mass; }  // 0.0f is infinite mass, kinematic object.
    glm::mat4 InvTensor();

    virtual void AddLinearImpulse(const glm::vec3& impulse);
    virtual void AddRotationalImpulse(const glm::vec3& point, const glm::vec3& impulse);
  
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 velocity  = { 0.0f, 0.0f, 0.0f };
    glm::vec3 orientation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 angVel = { 0.0f, 0.0f, 0.0f };
    glm::vec3 forces = { 0.0f, 0.0f, 0.0f };
    glm::vec3 torques = { 0.0f, 0.0f, 0.0f };

    float mass;
    float cor;  // coefficient of restitution

    #ifdef DYNAMIC_FRICTION
      float staticFriction;
      float dynamicFriction;
    #else
      float friction;
    #endif

    // DeRigidbodyVolume.orientation: axis-angle, degrees.
    // DeRigidbodyVolume.box.orientation: rotation matrix, radians.
    // Maybe just make them into single unit.
    OBB box;
    Sphere sphere;
};

CollisionManifold FindCollisionFeatures(DeRigidbodyVolume& ra, DeRigidbodyVolume& rb);
void ApplyImpulse(DeRigidbodyVolume& A, DeRigidbodyVolume& B, const CollisionManifold& M, int c);
