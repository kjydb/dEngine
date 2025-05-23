#pragma once

#include "de_game_object.hpp"
#include "de_window.hpp"

namespace de {

class KeyboardMovementController {
  public:
    struct KeyMappings {
      int moveLeft = GLFW_KEY_A;
      int moveRight = GLFW_KEY_D;
      int moveForward = GLFW_KEY_W;
      int moveBackward = GLFW_KEY_S;
      int moveUp = GLFW_KEY_E;
      int moveDown = GLFW_KEY_Q;
      int lookLeft = GLFW_KEY_LEFT;
      int lookRight = GLFW_KEY_RIGHT;
      int lookUp = GLFW_KEY_UP;
      int lookDown = GLFW_KEY_DOWN;
    };

    void addMousePosCallback( GLFWwindow* window );
    void addMouseButtonCallback( GLFWwindow* window );
    void moveInPlaneXZ(GLFWwindow* window, float dt, DeGameObject& gameObject);

    KeyMappings keys{};
    float moveSpeed{3.f};
    float lookSpeed{1.5f};

  private:
    static void mousePosCallback( GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
    static void lookAround( GLFWwindow* window );

    static int windowWidth;
    static int windowHeight;
    static double xpos;
    static double ypos;
    static double lastXpos;
    static double lastYpos;
    static double dx;
    static double dy;
    static bool rightMousePressed;

    static float sensitivity;
};

}  // namespace de
