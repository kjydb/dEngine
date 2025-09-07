#pragma once

#include "graphics/de_camera.hpp"
#include "graphics/de_shader.hpp"
#include "physics/de_geometry.hpp"
#include "physics/de_rigidbodyVolume.hpp"

class DeRenderSystem {
  public:
    void initPrimRender();

    void Render(std::vector<DeRigidbodyVolume> bodies, DeCamera camera, float aspect);
  
  private:
    void Render(const Point& point);
    void Render(const Line& line);
    void Render(const Ray& ray);
    void Render(const Sphere& sphere, glm::mat4 view, glm::mat4 projection);
    void Render(const AABB& aabb);
    void RenderWithQuads(const AABB& aabb);
    void Render(const OBB& obb, glm::mat4 view, glm::mat4 projection);
    void Render(const Plane& plane);
    void Render(const Plane& plane, float scale);
    // void Render(const Triangle& triangle);
    // void Render(const Triangle& triangle, bool oneSided);
    // void Render(const Frustum& frustum);
    // void RenderNormals(const Frustum& frustum);
    void Render(const CollisionManifold& manifold);

    DeShader prim_cubeShader = DeShader("shaders/primCube.vert", "shaders/primCube.frag");
    unsigned int prim_cubeVAO, prim_cubeVBO;
    float prim_cubeVertices[216] = {
          // positions          // colors
          -0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f,  1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 1.0f,
          -0.5f,  0.5f, -0.5f,  1.0f,  0.0f, 1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f,

          -0.5f, -0.5f,  0.5f,  1.0f,  0.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 1.0f,
          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

          -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
          -0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
          -0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,

          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
};
