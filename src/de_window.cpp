#include "de_window.hpp"

// std
#include <stdexcept>

namespace de {
    DeWindow::DeWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
        initWindow();
    }

    DeWindow::~DeWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void DeWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void DeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }

    void DeWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto deWindow = reinterpret_cast<DeWindow *>(glfwGetWindowUserPointer(window));
        deWindow->framebufferResized = true;
        deWindow->width = width;
        deWindow->height = height;
    }
}  // namespace De