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

class PointLightSystem {
  public:
    PointLightSystem( DeDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout );
    ~PointLightSystem();

    PointLightSystem( const PointLightSystem & ) = delete;
    PointLightSystem &operator=( const PointLightSystem & ) = delete;

    void update( FrameInfo &frameInfo, GlobalUbo &ubo );
    void render( FrameInfo &frameInfo );

  private:
    void createPipelineLayout( VkDescriptorSetLayout globalSetLayout );
    void createPipeline( VkRenderPass renderPass );

    DeDevice &deDevice;

    std::unique_ptr<DePipeline> dePipeline;
    VkPipelineLayout pipelineLayout;
};

}  // namespace de
