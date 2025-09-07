#include "cubeprim_test.hpp"

void CubePrim::initCube() {
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));

  cubeShader.use();
  cubeShader.setInt("cube", 0);
}

void CubePrim::updateCube(DeCamera camera, float aspect) {
  // cubeShader.use();
  // glm::mat4 primCubeModel = glm::mat4(1.0f);
  // glm::mat4 primCubeView = camera.GetViewMatrix();
  // glm::mat4 primCubeProjection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);
  // cubeShader.setMat4("model", primCubeModel);
  // cubeShader.setMat4("view", primCubeView);
  // cubeShader.setMat4("projection", primCubeProjection);
  // cubeShader.setVec3("cameraPos", camera.position);

  // glBindVertexArray(cubeVAO);
  // glActiveTexture(GL_TEXTURE0);
  // glDrawArrays(GL_TRIANGLES, 0, 36);
  // glBindVertexArray(0);

  glm::mat4 cubeModel(1.0f);
  glm::mat4 cubeView = camera.GetViewMatrix();
  glm::mat4 cubeProj = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);
  glm::vec3 cubePosition = { 0.0f, 0.0f, 0.0f };

  cubeShader.use();
  cubeShader.setMat4("cubeView", cubeView);
  cubeShader.setMat4("cubeProj", cubeProj);
  cubeShader.setMat4("cubeModel", cubeModel);

  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}
