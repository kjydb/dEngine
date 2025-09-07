#pragma once

#include <vector>

#include "physics/de_cloth.hpp"
#include "physics/de_geometry.hpp"
#include "physics/de_rigidbody.hpp"
#include "physics/de_spring.hpp"

class DePhysicsSystem {
  public:
    DePhysicsSystem();

    void Update(float dt);
    void Render();

    void AddRigidbody(DeRigidbody* body);
    void AddCloth(DeCloth* cloth);
    void AddSpring(const DeSpring& spring);
    void AddConstraint(const OBB& constraint);

    void ClearRigidbodies();
    void ClearCloths();
    void ClearSprings();
    void ClearConstraints();

    float LinearProjectionPercent;  // [0.2, 0.8], smaller = less jitter / more penetration
    float PenetrationSlack;  // [0.01, 0.1], smaller = more accurate
    int ImpulseIteration;

    bool DebugRender;
    bool DoLinearProjection;
    bool RenderRandomColors;

  protected:
    std::vector<DeRigidbody*> bodies;
    std::vector<DeCloth*> cloths;
    std::vector<DeSpring> springs;
    std::vector<OBB> constraints;

    std::vector<DeRigidbody*> colliders1;
    std::vector<DeRigidbody*> colliders2;
    std::vector<CollisionManifold> results;
};
