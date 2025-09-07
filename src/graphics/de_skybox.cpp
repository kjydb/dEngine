#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "graphics/de_skybox.hpp"

void DeSkybox::updateSkybox(DeCamera camera, float aspect) {
  /**
   * Cube in the middle.
   * delete this and associated variables later.
   */
  // cubeShader.use();
  // glm::mat4 skyboxModel = glm::mat4(1.0f);
  // glm::mat4 skyboxView = camera.GetViewMatrix();
  // glm::mat4 skyboxProjection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);
  // cubeShader.setMat4("model", skyboxModel);
  // cubeShader.setMat4("view", skyboxView);
  // cubeShader.setMat4("projection", skyboxProjection);
  // cubeShader.setVec3("cameraPos", camera.position);

  // glBindVertexArray(cubeVAO);
  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  // glDrawArrays(GL_TRIANGLES, 0, 36);
  // glBindVertexArray(0);

  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
  skyboxShader.use();
  glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));  // remove translation from the view matrix
  glm::mat4 skyboxProjection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);
  skyboxShader.setMat4("view", skyboxView);
  skyboxShader.setMat4("projection", skyboxProjection);

  glBindVertexArray(skyboxVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
}

void DeSkybox::initSkybox() {
  // cube VAO
  // 왜 여기 지우면 스카이박스 로드 안됨?
  // glGenVertexArrays(1, &cubeVAO);
  // glGenBuffers(1, &cubeVBO);
  // glBindVertexArray(cubeVAO);
  // glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));

  // skybox VAO
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

  unsigned int cubemapTexture = loadCubemap(faces);

  cubeShader.use();
  cubeShader.setInt("skybox", 0);

  skyboxShader.use();
  skyboxShader.setInt("skybox", 0);
}

unsigned int DeSkybox::loadCubemap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); ++i) {
    unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
      stbi_image_free(data);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP ,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}
