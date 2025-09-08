#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

#include "graphics/de_renderSystem.hpp"
#include "utils/shader_util.hpp"

void DeRenderSystem::initPrimRender() {
  prim_cubeVert = load_text_file("shaders/primCube.vert");
  prim_cubeFrag = load_text_file("shaders/primCube.frag");
  prim_cubeProg = link(compile(GL_VERTEX_SHADER, prim_cubeVert.c_str()),
                       compile(GL_FRAGMENT_SHADER, prim_cubeFrag.c_str()));
    
  glGenVertexArrays(1, &prim_cubeVAO);
  glBindVertexArray(prim_cubeVAO);

  glGenBuffers(1, &prim_cubeVBO);
  glBindBuffer(GL_ARRAY_BUFFER, prim_cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(prim_cubeVertices), &prim_cubeVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void DeRenderSystem::Render(Objects objects, DeCamera camera, float aspect) {
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);

  for (int i = 0, size = objects.bodies.size(); i < size; ++i) {
    if (objects.bodies[i].type == RIGIDBODY_TYPE_SPHERE) {
      Render(objects.bodies[i].sphere, view, projection);
    }
    else if (objects.bodies[i].type == RIGIDBODY_TYPE_BOX) {
      Render(objects.bodies[i].box, view, projection);
    }
  }

  for (int i = 0, size = objects.constraints.size(); i < size; ++i) {
    Render(objects.constraints[i], view, projection);
  }
}

void DeRenderSystem::Render(const Sphere& sphere, glm::mat4 view, glm::mat4 projection) {

}

void DeRenderSystem::Render(const OBB& obb, glm::mat4 view, glm::mat4 projection) {
  // TODO: Optimize this.
  glm::mat4 rotMat = glm::mat4(
    glm::vec4(obb.orientation[0], 0.0f),
    glm::vec4(obb.orientation[1], 0.0f),
    glm::vec4(obb.orientation[2], 0.0f),
    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
  );
  glm::mat4 cubeModel = glm::translate(glm::mat4(1.0f), obb.position) * rotMat * glm::scale(glm::mat4(1.0f), obb.size);

  glm::mat4 mvp = projection * view * cubeModel;

  glUseProgram(prim_cubeProg);
  int loc = glGetUniformLocation(prim_cubeProg, "uMVP");
  glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]);

  glBindVertexArray(prim_cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}
