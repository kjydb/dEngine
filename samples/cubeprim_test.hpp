#pragma once

#include "glad/glad.h"

#include "graphics/de_camera.hpp"
#include "graphics/de_shader.hpp"

class CubePrim {
  public:
    void initCube();
    void updateCube(DeCamera camera, float aspect);

    DeShader cubeShader = DeShader("shaders/primCube.vert", "shaders/primCube.frag");

  private:
    unsigned int cubeVAO, cubeVBO;

    float cubeVertices[216] = {
      // positions          // colors
      -0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  1.0f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,

      -0.5f, -0.5f,  0.5f,  1.0f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f,  0.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

      -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,

      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
};
