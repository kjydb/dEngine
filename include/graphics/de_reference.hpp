#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "graphics/de_camera.hpp"

struct Vtx { float x, y, z,  r, g, b; };

class DeReference {
  public:

    void initAxisGizmo();
    void updateGizmo(DeCamera camera, float aspect);
    void drawAxisGizmo(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& pos, float axisLength = 0.2f, bool onTop = false);

  private:
    GLuint axisVAO = 0;
    GLuint axisVBO = 0;

    std::string axisVSrc;
    std::string axisFSrc;
    GLuint axisProgram;
};
