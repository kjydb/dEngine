#pragma once

#include <glad/glad.h>

#include "graphics/de_renderSystem.hpp"
#include "physics/de_physicsSystem.hpp"
// #include "physics/de_rigidbodyVolume.hpp"

class DemoBase {
  public:
    DemoBase();

    void Initialize();
    void Update(float dt);
    void Render();

    Objects objects;

  private:
    void ResetScene();
    void SyncPhysics();
    void Demo1();
    void Demo2();

    DePhysicsSystem physicsSystem;

};
