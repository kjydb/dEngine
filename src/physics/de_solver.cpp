#include "de_solver.hpp"

namespace de {

void DeSolver::step(FrameInfo frameInfo) {
  for (auto& gameObject : frameInfo.gameObjects) {
    DeGameObject* obj = &gameObject.second;
    if (obj->model == nullptr) continue;

    obj->physics.force += obj->physics.mass * DeEnvironment::d_gravity;
    obj->physics.velocity += obj->physics.force / obj->physics.mass * frameInfo.frameTime;
    obj->physics.translation += obj->physics.velocity * frameInfo.frameTime;

    obj->physics.force = glm::vec3(0, 0, 0);
  }
}

}  // namespace de
