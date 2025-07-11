#include "movement_controller.hpp"

// std
#include <limits>
#include <iostream>

namespace de {

int KeyboardMovementController::windowWidth = 0;
int KeyboardMovementController::windowHeight = 0;
double KeyboardMovementController::xpos = 0;
double KeyboardMovementController::ypos = 0;
double KeyboardMovementController::lastXpos = 0;
double KeyboardMovementController::lastYpos = 0;
double KeyboardMovementController::dx = 0;
double KeyboardMovementController::dy = 0;
bool KeyboardMovementController::rightMousePressed = false;

void KeyboardMovementController::moveInPlaneXZ(
    GLFWwindow* window, float dt, DeGameObject& gameObject) {
  glm::vec3 rotate{ 0 };

  lookAround(window);

  if (rightMousePressed) {
    rotate.x += dy;
    rotate.y += dx;
  }

  if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    gameObject.physics.rotation += sensitivity * dt * rotate;
  }

  // limit pitch values between about +/- 85ish degrees
  gameObject.physics.rotation.x = glm::clamp(gameObject.physics.rotation.x, -1.5f, 1.5f);
  gameObject.physics.rotation.y = glm::mod(gameObject.physics.rotation.y, glm::two_pi<float>());

  float yaw = gameObject.physics.rotation.y;
  const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
  const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
  const glm::vec3 upDir{0.f, -1.f, 0.f};

  glm::vec3 moveDir{0.f};
  if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
  if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
  if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
  if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
  if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
  if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

  if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
    gameObject.physics.translation += moveSpeed * dt * glm::normalize(moveDir);
  }
}

void KeyboardMovementController::addMousePosCallback(GLFWwindow* window) {
  glfwSetCursorPosCallback(window, mousePosCallback);
}

void KeyboardMovementController::addMouseButtonCallback(GLFWwindow* window) {
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

void KeyboardMovementController::mousePosCallback(GLFWwindow* window, double xpos, double ypos) {

}

void KeyboardMovementController::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      rightMousePressed = true;
    } else {
      rightMousePressed = false;
    }
  }
}

// Press right mouse button and drag to look around
void KeyboardMovementController::lookAround(GLFWwindow* window) {
  glfwGetCursorPos(window, &xpos, &ypos);
  if (rightMousePressed) {
    dx = xpos - lastXpos;
    dy = lastYpos - ypos;
  }
  lastXpos = xpos;
  lastYpos = ypos;
}

}  // namespace de
