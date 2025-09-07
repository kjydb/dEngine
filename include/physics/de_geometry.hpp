#pragma once

#include <vector>

#include "glm/glm.hpp"

typedef glm::vec3 Point;

typedef struct Line {
  Point start;
  Point end;

  inline Line() {}
  inline Line(const Point& s, const Point& e) :
    start(s), end(e) { }
} Line;

typedef struct Ray {
  Point origin;
  glm::vec3 direction;

  inline Ray() : direction(0.0f, 0.0f, 0.0f) { }
  inline Ray(const Point& o, const glm::vec3& d) :
    origin(o), direction(d) {
    NormalizeDirection();
  }
  inline void NormalizeDirection() {
    glm::normalize(direction);
  }
} Ray;

typedef struct Sphere {
  Point position;
  float radius;

  inline Sphere() : radius(1.0f) { }
  inline Sphere(const Point& p, float r) :
    position(p), radius(r) { }
} Sphere;

typedef struct AABB {
  Point position;
  glm::vec3 size;  // half size

  inline AABB() : size(1.0f, 1.0f, 1.0f) { }
  inline AABB(const Point& p, const glm::vec3& s) :
    position(s), size(s) { }
} AABB;

typedef struct OBB {
  Point position;
  glm::vec3 size; // half size
  glm::mat3 orientation;

  inline OBB() : size(1.0f, 1.0f, 1.0f) { }
  inline OBB(const Point& p, const glm::vec3& s) :
    position(p), size(s) { }
  inline OBB(const Point& p, const glm::vec3& s, const glm::mat3& o) :
    position(p), size(s), orientation(o) { }
} OBB;

typedef struct Plane {
  glm::vec3 normal;
  float distance;

  inline Plane() : normal(1.0f, 0.0f, 0.0f) { }
  inline Plane(const glm::vec3& n, float d) : normal(n), distance(d) { }
} Plane;

typedef struct Interval {
  float min;
  float max;
} Interval;

bool PointInOBB(const Point& point, const OBB& obb);

Point ClosestPoint(const Sphere& sphere, const Point& point);
Point ClosestPoint(const AABB& aabb, const Point& point);
Point ClosestPoint(const OBB& obb, const Point& point);
Point ClosestPoint(const Plane& plane, const Point& point);
Point ClosestPoint(const Line& line, const Point& point);
Point ClosestPoint(const Ray& ray, const Point& point);

Point ClosestPoint(const Point& point, const Sphere& sphere);
Point ClosestPoint(const Point& point, const AABB& aabb);
Point ClosestPoint(const Point& point, const OBB& obb);
Point ClosestPoint(const Point& point, const Plane& plane);
Point ClosestPoint(const Point& point, const Line& line);
Point ClosestPoint(const Point& point, const Ray& ray);
// Point ClosestPoint(const Point& p, const Triangle& t);

Interval GetInterval(const AABB& aabb, const glm::vec3& axis);
Interval GetInterval(const OBB& obb, const glm::vec3& axis);
// Interval GetInterval(const Triangle& triangle, const glm::vec3& axis);

bool SphereSphere(const Sphere& s1, const Sphere& s2);
bool SphereAABB(const Sphere& sphere, const AABB& aabb);
bool SphereOBB(const Sphere& sphere, const OBB& obb);
bool SpherePlane(const Sphere& sphere, const Plane& plane);
bool AABBAABB(const AABB& aabb1, const AABB& aabb2);
bool AABBOBB(const AABB& aabb, const OBB& obb);
bool AABBPlane(const AABB& aabb, const Plane& plane);
bool OBBOBB(const OBB& obb1, const OBB& obb2);
bool OBBPlane(const OBB& obb, const Plane& plane);
bool PlanePlane(const Plane& plane1, const Plane& plane2);

typedef struct CollisionManifold {
  bool colliding;
  glm::vec3 normal;
  float depth;
  std::vector<glm::vec3> contacts;
} CollisionManifold;

void ResetCollisionManifold(CollisionManifold* result);

std::vector<Point> GetVertices(const OBB& obb);
std::vector<Line> GetEdges(const OBB& obb);
std::vector<Plane> GetPlanes(const OBB& obb);
bool ClipToPlane(const Plane& plane, const Line& line, Point* outPoint);
std::vector<Point> ClipEdgesToOBB(const std::vector<Line>& edges, const OBB& obb);
float PenetrationDepth(const OBB& o1, const OBB& o2, const glm::vec3& axis, bool* outShouldFlip);

CollisionManifold FindCollisionFeatures(const Sphere& A, const Sphere& B);
CollisionManifold FindCollisionFeatures(const OBB& A, const Sphere& B);
CollisionManifold FindCollisionFeatures(const OBB& A, const OBB& B);
