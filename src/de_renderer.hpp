#pragma once

#include "de_device.hpp"
#include "de_swap_chain.hpp"
#include "de_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace de {
    class DeRenderer {
        public:
        DeRenderer(DeWindow &window, DeDevice &device);
        ~DeRenderer();

        DeRenderer(const DeRenderer &) = delete;
        DeRenderer &operator=(const DeRenderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const { return deSwapChain->getRenderPass(); }
        float getAspectRatio() const { return deSwapChain->extentAspectRatio(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        DeWindow &deWindow;
        DeDevice &deDevice;
        std::unique_ptr<DeSwapChain> deSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};
    };
}  // namespace de