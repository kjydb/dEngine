#pragma once

#include "de_device.hpp"

// std
#include <string>
#include <vector>

namespace de {

struct PipelineConfigInfo {
  PipelineConfigInfo() = default;
  PipelineConfigInfo( const PipelineConfigInfo& ) = delete;
  PipelineConfigInfo& operator=( const PipelineConfigInfo& ) = delete;

  std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  std::vector<VkDynamicState> dynamicStateEnables;
  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class DePipeline {
  public:
    DePipeline( DeDevice& device, 
                const std::string& vertFilepath,  
                const std::string& fragFilepath, 
                const PipelineConfigInfo& configInfo );
    ~DePipeline();

    DePipeline( const DePipeline& ) = delete;
    DePipeline& operator=( const DePipeline& ) = delete;

    void bind( VkCommandBuffer commandBuffer );

    static void defaultPipelineConfigInfo( PipelineConfigInfo& configInfo );
    static void enableAlphaBlending( PipelineConfigInfo& configInfo );

  private:
    static std::vector<char> readFile( const std::string& filepath );

    void createGraphicsPipeline( const std::string& vertFilepath, 
                                 const std::string& fragFilepath, 
                                 const PipelineConfigInfo& configInfo );
    void createShaderModule( const std::vector<char>& code, VkShaderModule* shaderModule );

    DeDevice& deDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};

}  // namespace de
