#include "first_app.hpp"

#include "movement_controller.hpp"
#include "de_buffer.hpp"
#include "de_camera.hpp"
#include "systems/point_light_system.hpp"
#include "systems/simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <iostream>

namespace de {

FirstApp::FirstApp() {
  globalPool = 
    DeDescriptorPool::Builder(deDevice)
      .setMaxSets(DeSwapChain::MAX_FRAMES_IN_FLIGHT)
      .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, DeSwapChain::MAX_FRAMES_IN_FLIGHT)
      .build();
  loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
  std::vector<std::unique_ptr<DeBuffer>> uboBuffers(DeSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < uboBuffers.size(); i++) {
    uboBuffers[i] = std::make_unique<DeBuffer>(
      deDevice, 
      sizeof(GlobalUbo), 
      1, 
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    uboBuffers[i]->map();
  }

  auto globalSetLayout = 
    DeDescriptorSetLayout::Builder(deDevice)
      .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
      .build();
    
  std::vector<VkDescriptorSet> globalDescriptorSets(DeSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < globalDescriptorSets.size(); i++) {
    auto bufferInfo = uboBuffers[i]->descriptorInfo();
    DeDescriptorWriter(*globalSetLayout, *globalPool)
      .writeBuffer(0, &bufferInfo)
      .build(globalDescriptorSets[i]);
  }

  SimpleRenderSystem simpleRenderSystem{
    deDevice, 
    deRenderer.getSwapChainRenderPass(), 
    globalSetLayout->getDescriptorSetLayout()};
  PointLightSystem pointLightSystem{
    deDevice, 
    deRenderer.getSwapChainRenderPass(), 
    globalSetLayout->getDescriptorSetLayout()};
  DeCamera camera{};

  auto viewerObject = DeGameObject::createGameObject();
  viewerObject.transform.translation.z = -2.5f;
  KeyboardMovementController cameraController{};

  cameraController.addMouseButtonCallback(deWindow.getGLFWwindow());

  auto currentTime = std::chrono::high_resolution_clock::now();
  while (!deWindow.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime = 
      std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    cameraController.moveInPlaneXZ(deWindow.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    float aspect = deRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

    if (auto commandBuffer = deRenderer.beginFrame()) {
      int frameIndex = deRenderer.getFrameIndex();
      FrameInfo frameInfo{
        frameIndex, 
        frameTime, 
        commandBuffer, 
        camera, 
        globalDescriptorSets[frameIndex], 
        gameObjects};

      // update
      GlobalUbo ubo{};
      ubo.projection = camera.getProjection();
      ubo.view = camera.getView();
      ubo.inverseView = camera.getInverseView();
      pointLightSystem.update(frameInfo, ubo);
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();

      // render
      deRenderer.beginSwapChainRenderPass(commandBuffer);

      // order here matters
      simpleRenderSystem.renderGameObjects(frameInfo);
      pointLightSystem.render(frameInfo);

      deRenderer.endSwapChainRenderPass(commandBuffer);
      deRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(deDevice.device());
}

void FirstApp::loadGameObjects() {
  std::shared_ptr<DeModel> deModel = 
    DeModel::createModelFromFile(deDevice, "models/flat_vase.obj");
  auto flatVase = DeGameObject::createGameObject();
  flatVase.model = deModel;
  flatVase.transform.translation = { -.5f, .5f, 0.f };
  flatVase.transform.scale = { 3.f, 1.5f, 3.f };
  gameObjects.emplace(flatVase.getId(), std::move(flatVase));

  deModel = DeModel::createModelFromFile(deDevice, "models/smooth_vase.obj");
  auto smoothVase = DeGameObject::createGameObject();
  smoothVase.model = deModel;
  smoothVase.transform.translation = { .5f, .5f, 0.f };
  smoothVase.transform.scale = { 3.f, 1.5f, 3.f };
  gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

  deModel = DeModel::createModelFromFile(deDevice, "models/quad.obj");
  auto floor = DeGameObject::createGameObject();
  floor.model = deModel;
  floor.transform.translation = { 0.f, .5f, 0.f };
  floor.transform.scale = { 3.f, 1.f, 3.f };
  gameObjects.emplace(floor.getId(), std::move(floor));

  std::vector<glm::vec3> lightColors{
    { 1.f, .1f, .1f }, 
    { .1f, .1f, 1.f }, 
    { .1f, 1.f, .1f }, 
    { 1.f, 1.f, .1f }, 
    { .1f, 1.f, 1.f }, 
    { 1.f, 1.f, 1.f }
  };

  for (int i = 0; i < lightColors.size(); i++) {
    auto pointLight = DeGameObject::makePointLight(0.2f);
    pointLight.color = lightColors[i];
    auto rotateLight = glm::rotate(
      glm::mat4(1.f), 
      (i * glm::two_pi<float>()) / lightColors.size(), 
      {0.f, -1.f, 0.f});
    pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
    gameObjects.emplace(pointLight.getId(), std::move(pointLight));
  }
}
  
}  // namespace de
