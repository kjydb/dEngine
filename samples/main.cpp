#include <cstdio>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLM: header-only
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "graphics/de_camera.hpp"
#include "graphics/de_reference.hpp"
#include "graphics/de_renderSystem.hpp"
#include "graphics/de_skybox.hpp"
#include "utils/shader_util.hpp"

#include "physics/de_physicsSystem.hpp"
#include "physics/de_rigidbodyVolume.hpp"

#include "demos.hpp"

namespace {
  GLFWwindow* mainWindow = nullptr;

  // Camera
  DeCamera camera = DeCamera(glm::vec3(0.0f, 0.0f, 0.0f));

  // Reference
  DeReference reference;

  // Physics
  DePhysicsSystem physicsSystem;

  DemoBase demo;

  float currentTime = 0.0f;
  float dt = 0.0f;
  float prevTime = 0.0f;

  float timeStep = 1.0f / 60.0f;

  int width = 1600;
  int height = 1200;
}

static void glfwErrorCallback(int error, const char* description) {
  printf("GLFW error %d: %s\n", error, description);
}

static void windowSizeCallback(GLFWwindow*, int w, int h) {
  width = w;
  height = h > 0 ? h : 1;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

static void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
  glViewport(0, 0, w, h);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      camera.rightMousePressed = true;
    } else {
      camera.rightMousePressed = false;
    }
  }
}

static void DrawText(int x, int y, const char* string) {
  ImVec2 p;
  p.x = float(x);
  p.y = float(y);
  ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
  ImGui::SetCursorPos(p);
  ImGui::TextColored(ImColor(230, 153, 153, 255), "%s", string);
  ImGui::End();
}

int main() {
  glfwSetErrorCallback(glfwErrorCallback);

  if (glfwInit() == 0) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  // Change openGL version if necessary.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  mainWindow = glfwCreateWindow(width, height, "opengl_tutorial", NULL, NULL);
  if (mainWindow == NULL) {
    fprintf(stderr, "Failed to open GLFW mainWindow.\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(mainWindow);
  glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

  // Load GLAD.
  int gladStatus = gladLoadGL();
  if (gladStatus == 0) {
    fprintf(stderr, "Failed to load OpenGL.\n");
    glfwTerminate();
    return -1;
  }

  glfwSwapInterval(1);
  glfwSetWindowSizeCallback(mainWindow, windowSizeCallback);
  glfwSetMouseButtonCallback(mainWindow, mouseButtonCallback);

  float xscale, yscale;
  glfwGetWindowContentScale(mainWindow, &xscale, &yscale);

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsClassic();
  ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
  ImGui_ImplOpenGL3_Init();
  ImGuiIO& io = ImGui::GetIO();
  float uiScale = xscale;
  io.FontGlobalScale = uiScale;

  // 5) 깊이 테스트
  glEnable(GL_DEPTH_TEST);

  // Render System
  DeRenderSystem renderSystem;
  renderSystem.initPrimRender();

  // Skybox
  DeSkybox skybox = DeSkybox();
  skybox.initSkybox();

  // init gizmo
  reference.initAxisGizmo();

  demo.Initialize();

  // main loop
  while (!glfwWindowShouldClose(mainWindow)) {
    currentTime = static_cast<float>(glfwGetTime());
    dt = currentTime - prevTime;
    prevTime = currentTime;

    int w, h;
    glfwGetFramebufferSize(mainWindow, &w, &h);
    float aspect = (h == 0) ? 1.0f : (float)w / (float)h;

    glViewport(0, 0, w, h);
    glClearColor(0.08f, 0.09f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Globally position text
    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f));
    ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
    ImGui::End();

    char buffer[64];
    sprintf(buffer, "frame time: %f", dt);
    DrawText(5, 5, buffer);

    sprintf(buffer, "LinearProjectionPercent: %f", physicsSystem.LinearProjectionPercent);
    DrawText(5, 35, buffer);

    sprintf(buffer, "PanetrationSlack: %f", physicsSystem.PenetrationSlack);
    DrawText(5, 65, buffer);

    sprintf(buffer, "ImpulseIteration: %d", physicsSystem.ImpulseIteration);
    DrawText(5, 95, buffer);

    // physics
    demo.Update(dt);

    // Render prims
    renderSystem.Render(demo.objects, camera, aspect);

    // camera operation.
    camera.lookAround(mainWindow, dt);
    camera.move(mainWindow, dt);

    // gizmo
    reference.updateGizmo(camera, aspect);

    // skybox
    skybox.updateSkybox(camera, aspect);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(mainWindow);
    glfwPollEvents();
  }

  glfwDestroyWindow(mainWindow);
  glfwTerminate();
  return 0;
}
