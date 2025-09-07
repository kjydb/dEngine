#include <iostream>

#include "graphics/de_camera.hpp"
#include "GLFW/glfw3.h"


void DeCamera::rotate_camera() {
  const float radius = 10.0f;
  float camX = sin(glfwGetTime()) * radius;
  float camZ = cos(glfwGetTime()) * radius;
  glm::mat4 view;
  view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void DeCamera::initCamera() {
  
}

void DeCamera::move(int key, float dt) {
  float cameraSpeed = 2.5f * dt;

  printf("position: { %f, %f, %f }\n", position.x, position.y, position.z);
  
  if (key == GLFW_KEY_W) {
    position += cameraSpeed * Forward;
    printf("W pressed.\n");
  }
  if (key == GLFW_KEY_S) {
    position -= cameraSpeed * Forward;
    printf("S pressed.\n");
  }
  if (key == GLFW_KEY_A) {
    position -= glm::normalize(glm::cross(Forward, Up)) * cameraSpeed;
    printf("A pressed.\n");
  }
  if (key == GLFW_KEY_D) {
    position += glm::normalize(glm::cross(Forward, Up)) * cameraSpeed;
    printf("D pressed.\n");
  }
  if (key == GLFW_KEY_E) {
    position += cameraSpeed * Up;
    printf("E pressed.\n");
  }
  if (key == GLFW_KEY_Q) {
    position -= cameraSpeed * Up;
    printf("Q pressed.\n");
  }
}

void DeCamera::move(GLFWwindow* window, float dt) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    printf("Closing program.\n");
    glfwSetWindowShouldClose(window, true);
  }

  glm::vec3 moveDir{ 0.0f };
  if (glfwGetKey(window, key.moveForward) == GLFW_PRESS) moveDir += Forward;
  if (glfwGetKey(window, key.moveBackward) == GLFW_PRESS) moveDir -= Forward;
  if (glfwGetKey(window, key.moveLeft) == GLFW_PRESS) moveDir -= Right;
  if (glfwGetKey(window, key.moveRight) == GLFW_PRESS) moveDir += Right;
  if (glfwGetKey(window, key.moveUp) == GLFW_PRESS) moveDir += Up;
  if (glfwGetKey(window, key.moveDown) == GLFW_PRESS) moveDir -= Up;

  if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
    position += Speed * dt * glm::normalize(moveDir);
}

void DeCamera::lookAround(GLFWwindow* window, float dt) {
  glm::vec3 rotate { 0 };

  glfwGetCursorPos(window, &mouseXPos, &mouseYPos);
  rotate = getMouseDiff(rotate);

  orientation += Sensitivity * dt * rotate;  // check if possible minimum value check is needed.

  // Limit viewing angle. Delete this if not needed.
  orientation.x = glm::mod(orientation.x, glm::two_pi<float>());  // yaw
  orientation.y = glm::clamp(orientation.y, -1.57f, 1.57f);  // pitch

  updateCameraVector();
}

glm::vec3 DeCamera::getMouseDiff(glm::vec3 rotate) {
  if (rightMousePressed) {
    rotate.x = lastMouseXPos - mouseXPos;
    rotate.y = lastMouseYPos - mouseYPos;  // Check if the skybox is inverted. If so, apply (-) operation
  }
  lastMouseXPos = mouseXPos;
  lastMouseYPos = mouseYPos;

  return rotate;
}

void DeCamera::updateCameraVector() {
  // https://stackoverflow.com/questions/1568568/how-to-convert-euler-angles-to-directional-vector
  // review this. 
  Forward = glm::vec3(cos(orientation.y)*sin(orientation.x), sin(orientation.y), cos(orientation.y)*cos(orientation.x));
  Right = glm::cross(Forward, WorldUp);
  Up = glm::cross(Right, Forward);
}
