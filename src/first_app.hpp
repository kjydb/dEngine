#pragma once

#include "de_descriptors.hpp"
#include "de_device.hpp"
#include "de_game_object.hpp"
#include "de_renderer.hpp"
#include "de_window.hpp"

// std
#include <memory>
#include <vector>

namespace de {
    class FirstApp {
        public:
        static constexpr int WIDTH = 1600;
        static constexpr int HEIGHT = 1200;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;

        void run();

        private:
        void loadGameObjects();

        DeWindow deWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
        DeDevice deDevice{deWindow};
        DeRenderer deRenderer{deWindow, deDevice};

        // npte: order of declarations matters
        std::unique_ptr<DeDescriptorPool> globalPool{};
        DeGameObject::Map gameObjects;
    };
}