#pragma once

#include "../de_camera.hpp"
#include "../de_device.hpp"
#include "../de_frame_info.hpp"
#include "../de_game_object.hpp"
#include "../de_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace de {

class SimpleRenderSystem {
  public:
    SimpleRenderSystem( DeDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout );
    ~SimpleRenderSystem();

    SimpleRenderSystem( const SimpleRenderSystem & ) = delete;
    SimpleRenderSystem &operator=( const SimpleRenderSystem & ) = delete;

    void renderGameObjects( FrameInfo &frameInfo );

  private:
    void createPipelineLayout( VkDescriptorSetLayout globalSetLayout );
    void createPipeline( VkRenderPass renderPass );

    DeDevice &deDevice;

    std::unique_ptr<DePipeline> dePipeline;
    VkPipelineLayout pipelineLayout;
};

}  // namespace de
