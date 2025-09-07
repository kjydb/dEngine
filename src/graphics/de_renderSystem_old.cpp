#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

#include "graphics/de_renderSystem.hpp"

void DeRenderSystem::initPrimRender() {
  glGenVertexArrays(1, &prim_cubeVAO);
  glGenBuffers(1, &prim_cubeVBO);
  glBindVertexArray(prim_cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, prim_cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(prim_cubeVertices), &prim_cubeVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));

  prim_cubeShader.use();
  prim_cubeShader.setInt("prim_cube", 0);
}

void DeRenderSystem::Render(std::vector<DeRigidbodyVolume> bodies, DeCamera camera, float aspect) {
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);

  for (int i = 0, size = bodies.size(); i < size; ++i) {
    if (bodies[i].type == RIGIDBODY_TYPE_SPHERE) {
      Render(bodies[i].sphere, view, projection);
    }
    else if (bodies[i].type == RIGIDBODY_TYPE_BOX) {
      Render(bodies[i].box, view, projection);
    }
  }
}

void DeRenderSystem::Render(const Sphere& sphere, glm::mat4 view, glm::mat4 projection) {

}

void DeRenderSystem::Render(const OBB& obb, glm::mat4 view, glm::mat4 projection) {
  glm::mat4 cubeModel(1.0f);
  cubeModel = glm::translate(cubeModel, obb.position);
  cubeModel = glm::scale(cubeModel, obb.size);

  prim_cubeShader.use();
  prim_cubeShader.setMat4("cubeView", view);
  prim_cubeShader.setMat4("cubeProj", projection);
  prim_cubeShader.setMat4("cubeModel", cubeModel);

  glBindVertexArray(prim_cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}
