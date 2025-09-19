#pragma once

#include "physics/de_rigidbody.hpp"

#define EULER_INTEGRATION
// #define ACCURATE_EULER_INTEGRATION

class DeParticle : public DeRigidbody {
  public:
    DeParticle();

    void Update(float dt);
    void ApplyForces();
    void SolveConstraints(const std::vector<OBB>& constraints);

    void SetPosition(const glm::vec3& pos);
    glm::vec3 GetPosition();

    void SetBounce(float b);
    float GetBounce();

    void AddImpulse(const glm::vec3& impulse);
    float InvMass();
    void SetMass(float m);
    glm::vec3 GetVelocity();
    void SetFriction(float f);

  private:
    glm::vec3 position;
    glm::vec3 oldPosition;
    glm::vec3 forces;

    glm::vec3 gravity;
    float friction;
    float bounce;

    #ifdef EULER_INTEGRATION
      glm::vec3 velocity;
    #endif

    float mass;


};
