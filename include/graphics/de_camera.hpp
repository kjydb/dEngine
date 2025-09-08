#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 90.0f;

// Movement keys
struct KeyMappings {
  int moveForward = GLFW_KEY_W;
  int moveBackward = GLFW_KEY_S;
  int moveLeft = GLFW_KEY_A;
  int moveRight = GLFW_KEY_D;
  int moveUp = GLFW_KEY_E;
  int moveDown = GLFW_KEY_Q;
};

class DeCamera {
  public:
    DeCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
             glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f), 
             glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
             float yaw = YAW, 
             float pitch = PITCH) 
      : position(position), 
        orientation(orientation), 
        Up(up), 
        Yaw(yaw), 
        Pitch(pitch), 
        Speed(SPEED), 
        Sensitivity(SENSITIVITY), 
        Zoom(ZOOM) {
      initCamera();
    }

    /**
     * positive x-direction is pointing right.
     * positive y-direction is pointing up.
     * positive z-direction is pointing myself from screen.
     * 
     * Front is orientation.
     * orientation is { yaw, pitch, roll }
     * 
     * I don't even know anymore.
     */
    inline glm::mat4 GetViewMatrix() { return glm::lookAt(position, position + Forward, WorldUp); }  // 왜 Forward로 해야됨? 왜 orientation 안됨?

    void move(int key, float dt);
    void move(GLFWwindow* window, float dt);
    void rotate_camera();
    void lookAround(GLFWwindow* window, float dt);
    void updateCameraVector();

    glm::vec3 position;
    glm::vec3 orientation;
    float Zoom;

    bool rightMousePressed = false;

  private:
    void initCamera();
    glm::vec3 getMouseDiff(glm::vec3 rotate);

    glm::vec3 Forward;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };

    float Yaw;
    float Pitch;
    float Speed;
    float Sensitivity;

    double mouseXPos;
    double mouseYPos;
    double lastMouseXPos;
    double lastMouseYPos;
    double dx;
    double dy;

    KeyMappings key{};
};