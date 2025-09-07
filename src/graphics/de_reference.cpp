#include <glm/gtc/matrix_transform.hpp>

#include "graphics/de_reference.hpp"
#include "utils/shader_util.hpp"

void DeReference::initAxisGizmo() {
  static Vtx axisVerts[] = {
    {-0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f},  // X axis
    {0.0f, -0.0f, 0.0f,  0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f},  // Y axis
    {0.0f, 0.0f, -0.0f,  0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f}   // Z axis
  };

  glGenVertexArrays(1, &axisVAO);
  glGenBuffers(1, &axisVBO);
  glBindVertexArray(axisVAO);
  glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(axisVerts), axisVerts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vtx), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vtx), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
  glEnable(GL_DEPTH_TEST);

  axisVSrc = load_text_file("shaders/reference.vert");
  axisFSrc = load_text_file("shaders/reference.frag");
  axisProgram = link(compile(GL_VERTEX_SHADER, axisVSrc.c_str()), 
                     compile(GL_FRAGMENT_SHADER, axisFSrc.c_str()));
}

void DeReference::updateGizmo(DeCamera camera, float aspect) {
  glm::mat4 referenceView = camera.GetViewMatrix();
  glm::mat4 referenceProj = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);
  glm::vec3 referencePosition = { 0.0f, 0.0f, 0.0f };

  drawAxisGizmo(referenceView, referenceProj, referencePosition);
}

void DeReference::drawAxisGizmo(
    const glm::mat4& view, 
    const glm::mat4& proj, 
    const glm::vec3& pos, 
    float axisLength, 
    bool onTop) {
  glm::mat4 model(1.0f);
  model = glm::translate(model, pos);
  model = glm::scale(model, glm::vec3(axisLength));

  glUseProgram(axisProgram);
  glUniformMatrix4fv(glGetUniformLocation(axisProgram, "uView"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(axisProgram, "uProj"), 1, GL_FALSE, glm::value_ptr(proj));
  glUniformMatrix4fv(glGetUniformLocation(axisProgram, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

  if (onTop) glDisable(GL_DEPTH_TEST);
  glBindVertexArray(axisVAO);
  glLineWidth(2.0f);
  glDrawArrays(GL_LINES, 0, 6);
  glBindVertexArray(0);
  if (onTop) glEnable(GL_DEPTH_TEST);
}
