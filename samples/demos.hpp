#pragma once

#include <glad/glad.h>

#include "graphics/de_renderSystem.hpp"
#include "physics/de_physicsSystem.hpp"

class DemoBase {
  public:
    DemoBase();

    void Initialize();
    void Update(float dt);
    void UpdateGUI();

    Objects objects;

  private:
    void ResetScene();
    void SyncPhysics();
    void LoadDemo();
    void Demo1();
    void Demo2();

    DePhysicsSystem physicsSystem;
    void (DemoBase::*demoArr[2])();
    int currentDemo = 0;

};
