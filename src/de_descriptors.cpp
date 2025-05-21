#include "de_descriptors.hpp"

// std
#include <cassert>
#include <stdexcept>

namespace de {

// Descriptor Set Layout Builder

DeDescriptorSetLayout::Builder &DeDescriptorSetLayout::Builder::addBinding(
    uint32_t binding, 
    VkDescriptorType descriptorType, 
    VkShaderStageFlags stageFlags, 
    uint32_t count) {
  assert(bindings.count(binding) == 0 && "Binding already in use");
  VkDescriptorSetLayoutBinding layoutBinding{};
  layoutBinding.binding = binding;
  layoutBinding.descriptorType = descriptorType;
  layoutBinding.descriptorCount = count;
  layoutBinding.stageFlags = stageFlags;
  bindings[binding] = layoutBinding;
  return *this;
}

std::unique_ptr<DeDescriptorSetLayout> DeDescriptorSetLayout::Builder::build() const {
  return std::make_unique<DeDescriptorSetLayout>(deDevice, bindings);
}

// Descriptor Set Layout

DeDescriptorSetLayout::DeDescriptorSetLayout(
    DeDevice &deDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
    : deDevice{ deDevice }, bindings{ bindings} {
  std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
  for (auto kv : bindings) {
    setLayoutBindings.push_back(kv.second);
  }

  VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
  descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
  descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

  if (vkCreateDescriptorSetLayout(
        deDevice.device(), 
        &descriptorSetLayoutInfo, 
        nullptr, 
        &descriptorSetLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor set layout!");
  }
}

DeDescriptorSetLayout::~DeDescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(deDevice.device(), descriptorSetLayout, nullptr);
}

// Descriptor Pool Builder

DeDescriptorPool::Builder &DeDescriptorPool::Builder::addPoolSize(
    VkDescriptorType descriptorType, uint32_t count) {
  poolSizes.push_back({descriptorType, count});
  return *this;
}

DeDescriptorPool::Builder &DeDescriptorPool::Builder::setPoolFlags(
    VkDescriptorPoolCreateFlags flags) {
  poolFlags = flags;
  return *this;
}

DeDescriptorPool::Builder &DeDescriptorPool::Builder::setMaxSets(uint32_t count) {
  maxSets = count;
  return *this;
}

std::unique_ptr<DeDescriptorPool> DeDescriptorPool::Builder::build() const {
  return std::make_unique<DeDescriptorPool>(deDevice, maxSets, poolFlags, poolSizes);
}

// DescriptorPool

DeDescriptorPool::DeDescriptorPool(
    DeDevice &deDevice, 
    uint32_t maxSets, 
    VkDescriptorPoolCreateFlags poolFlags, 
    const std::vector<VkDescriptorPoolSize> &poolSizes)
    : deDevice{ deDevice } {
      VkDescriptorPoolCreateInfo descriptorPoolInfo {};
  descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  descriptorPoolInfo.pPoolSizes = poolSizes.data();
  descriptorPoolInfo.maxSets = maxSets;
  descriptorPoolInfo.flags = poolFlags;

  if (vkCreateDescriptorPool(deDevice.device(), &descriptorPoolInfo, nullptr, &descriptorPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

DeDescriptorPool::~DeDescriptorPool() {
  vkDestroyDescriptorPool(deDevice.device(), descriptorPool, nullptr);
}

bool DeDescriptorPool::allocateDescriptor(
    const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const {
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptorPool;
  allocInfo.pSetLayouts = &descriptorSetLayout;
  allocInfo.descriptorSetCount = 1;

  // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
  // a new pool whenever an old pool fills up. But this is beyond our current scope
  if (vkAllocateDescriptorSets(deDevice.device(), &allocInfo, &descriptor) != VK_SUCCESS) {
    return false;
  }
  return true;
}

void DeDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const {
  vkFreeDescriptorSets(
    deDevice.device(), 
    descriptorPool, 
    static_cast<uint32_t>(descriptors.size()), 
    descriptors.data());
}

void DeDescriptorPool::resetPool() {
  vkResetDescriptorPool(deDevice.device(), descriptorPool, 0);
}

// Descriptor Writer

DeDescriptorWriter::DeDescriptorWriter(DeDescriptorSetLayout &setLayout, DeDescriptorPool &pool)
    : setLayout{ setLayout }, pool{ pool } {}
  
DeDescriptorWriter &DeDescriptorWriter::writeBuffer(
    uint32_t binding, VkDescriptorBufferInfo *bufferInfo) {
  assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

  auto &bindingDescription = setLayout.bindings[binding];

  assert(
    bindingDescription.descriptorCount == 1 &&
    "Binding single descriptor info, but binding expects multiple");
  
  VkWriteDescriptorSet write{};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.descriptorType = bindingDescription.descriptorType;
  write.dstBinding = binding;
  write.pBufferInfo = bufferInfo;
  write.descriptorCount = 1;

  writes.push_back(write);
  return *this;
}

DeDescriptorWriter &DeDescriptorWriter::writeImage(
    uint32_t binding, VkDescriptorImageInfo *imageInfo) {
  assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

  auto &bindingDescription = setLayout.bindings[binding];

  assert(
    bindingDescription.descriptorCount == 1 && 
    "Binding single descriptor info, but binding expects multiple");

  VkWriteDescriptorSet write{};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.descriptorType = bindingDescription.descriptorType;
  write.dstBinding = binding;
  write.pImageInfo = imageInfo;
  write.descriptorCount = 1;

  writes.push_back(write);
  return *this;
}

bool DeDescriptorWriter::build(VkDescriptorSet &set) {
  bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
  if (!success) {
    return false;
  }
  overwrite(set);
  return true;
}

void DeDescriptorWriter::overwrite(VkDescriptorSet &set) {
  for (auto &write : writes) {
    write.dstSet = set;
  }
  vkUpdateDescriptorSets(pool.deDevice.device(), writes.size(), writes.data(), 0, nullptr);
}

}  // namespace de
