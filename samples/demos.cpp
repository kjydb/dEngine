#include "demos.hpp"

DemoBase::DemoBase() {

}

void DemoBase::Initialize() {
  Demo2();
}

void DemoBase::Update(float dt) {
  physicsSystem.Update(dt);
}

void DemoBase::Render() {

}

void DemoBase::ResetScene() {
  physicsSystem.ClearRigidbodies();
  physicsSystem.ClearConstraints();
  objects.bodies.clear();
}

void DemoBase::SyncPhysics() {
  for (int i = 0; i < objects.bodies.size(); ++i) {
    objects.bodies[i].SyncCollisionVolumes();
    physicsSystem.AddRigidbody(&objects.bodies[i]);
  }
}

void DemoBase::Demo1() {
  ResetScene();
  objects.bodies.resize(3);

  objects.bodies[0].type = RIGIDBODY_TYPE_BOX;
  objects.bodies[0].position = glm::vec3(0.0f, -3.0f, 0.0f);
  objects.bodies[0].box.size = glm::vec3(10.0f, 0.5f, 10.0f);
  objects.bodies[0].mass = 0.0f;

  objects.bodies[1].type = RIGIDBODY_TYPE_BOX;
  objects.bodies[1].position = glm::vec3(-1.0f, 10.0f, 3.0f);
  objects.bodies[1].orientation = glm::vec3(0.0f, 0.0f, 0.0f);
  objects.bodies[1].box.size = glm::vec3(0.5f, 0.5f, 0.5f);

  objects.bodies[2].type = RIGIDBODY_TYPE_BOX;
  objects.bodies[2].position = glm::vec3(1.0f, 3.f, 3.0f);
  objects.bodies[2].orientation = glm::vec3(0.0f, 0.0f, 0.0f);
  objects.bodies[2].box.size = glm::vec3(0.5f, 0.5f, 0.5f);

  SyncPhysics();
}

void DemoBase::Demo2() {
  ResetScene();
  objects.bodies.resize(2);

  objects.bodies[0].type = RIGIDBODY_TYPE_BOX;
  objects.bodies[0].position = glm::vec3(0.0f, -3.0f, 0.0f);
  objects.bodies[0].box.size = glm::vec3(10.0f, 0.5f, 10.0f);
  objects.bodies[0].mass = 0.0f;

  objects.bodies[1].type = RIGIDBODY_TYPE_BOX;
  objects.bodies[1].position = glm::vec3(-1.0f, 0.0f, 3.0f);
  objects.bodies[1].orientation = glm::vec3(1.57f, 0.0f, 0.0f);
  objects.bodies[1].box.size = glm::vec3(0.5f, 0.5f, 0.5f);
  objects.bodies[1].mass = 0.0f;

  SyncPhysics();
}
