#pragma once

#include "de_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace de {

class DeDescriptorSetLayout {
  public:
    class Builder {
      public:
        Builder( DeDevice &deDevice ) : deDevice{ deDevice } {}

        Builder &addBinding(
          uint32_t binding, 
          VkDescriptorType descriptorType, 
          VkShaderStageFlags stageFlags, 
          uint32_t count = 1);
        std::unique_ptr<DeDescriptorSetLayout> build() const;

      private:
        DeDevice &deDevice;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
    };

    DeDescriptorSetLayout(
        DeDevice &deDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
    ~DeDescriptorSetLayout();
    DeDescriptorSetLayout( const DeDescriptorSetLayout & ) = delete;
    DeDescriptorSetLayout &operator=( const DeDescriptorSetLayout & ) = delete;

    VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
  
  private:
    DeDevice &deDevice;
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

    friend class DeDescriptorWriter;
};

class DeDescriptorPool {
  public:
    class Builder {
      public:
        Builder( DeDevice &deDevice ) : deDevice{ deDevice } {}

        Builder &addPoolSize( VkDescriptorType descriptorType, uint32_t count );
        Builder &setPoolFlags( VkDescriptorPoolCreateFlags flags );
        Builder &setMaxSets( uint32_t count);
        std::unique_ptr<DeDescriptorPool> build() const;

      private:
        DeDevice &deDevice;
        std::vector<VkDescriptorPoolSize> poolSizes{};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags poolFlags = 0;
    };

    DeDescriptorPool(
        DeDevice &deDevice, 
        uint32_t maxSets, 
        VkDescriptorPoolCreateFlags poolFlags, 
        const std::vector<VkDescriptorPoolSize> &poolSizes);
    ~DeDescriptorPool();
    DeDescriptorPool( const DeDescriptorPool & ) = delete;
    DeDescriptorPool &operator=( const DeDescriptorPool & ) = delete;

    bool allocateDescriptor(
        const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
    
    void freeDescriptors( std::vector<VkDescriptorSet> &descriptors ) const;

    void resetPool();

  private:
    DeDevice &deDevice;
    VkDescriptorPool descriptorPool;

    friend class DeDescriptorWriter;
};

class DeDescriptorWriter {
  public:
    DeDescriptorWriter( DeDescriptorSetLayout &setLayout, DeDescriptorPool &pool );

    DeDescriptorWriter &writeBuffer( uint32_t binding, VkDescriptorBufferInfo *bufferInfo );
    DeDescriptorWriter &writeImage( uint32_t binding, VkDescriptorImageInfo *imageInfo );

    bool build( VkDescriptorSet &set );
    void overwrite( VkDescriptorSet &set );

  private:
    DeDescriptorSetLayout &setLayout;
    DeDescriptorPool &pool;
    std::vector<VkWriteDescriptorSet> writes;
};

}  // namespace de
