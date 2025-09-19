#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "physics/de_geometry.hpp"

#define RIGIDBODY_TYPE_BASE     0
#define RIGIDBODY_TYPE_PARTICLE 1
#define RIGIDBODY_TYPE_SPHERE   2
#define RIGIDBODY_TYPE_BOX      3

class DeRigidbody {
  public:
    inline DeRigidbody() {
      type = RIGIDBODY_TYPE_BASE;
    }
    virtual inline ~DeRigidbody() { }

    virtual inline void Update(float dt) { }
    virtual inline void Render() { }
    virtual inline void ApplyForces() { }
    virtual inline void SolveConstraints(const std::vector<OBB> &constraints) { }

    int type;
    glm::vec3 color = { 255.0f, 255.0f, 255.0f };
  
  inline bool HasVolume() {
    return type == RIGIDBODY_TYPE_SPHERE || type == RIGIDBODY_TYPE_BOX;
  }
};
