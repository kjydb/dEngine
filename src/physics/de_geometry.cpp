#include <iostream>

#include "physics/de_geometry.hpp"
#include "utils/math_util.hpp"

bool PointInOBB(const Point& point, const OBB& obb) {
  glm::vec3 dir = point - obb.position;

  for (int i = 0; i < 3; ++i) {
    glm::vec3 axis(obb.orientation[i][0], obb.orientation[i][1], obb.orientation[i][2]);

    float distance = glm::dot(dir, axis);

    if (distance > obb.size[i]) {
      return false;
    }
    if (distance < -obb.size[i]) {
      return false;
    }
  }

  return true;
}

Point ClosestPoint(const OBB& obb, const Point& point) {
  Point result = obb.position;
  glm::vec3 dir = point - obb.position;

  for (int i = 0; i < 3; ++i) {
    const float* orientation = &obb.orientation[i][0];  // TODO: Check if this works
    glm::vec3 axis(orientation[0], orientation[1], orientation[2]);

    float distance = glm::dot(dir, axis);

    if (distance > obb.size[i]) {
      distance = obb.size[i];
    }
    if (distance < -obb.size[i]) {
      distance = -obb.size[i];
    }

    result = result + (axis * distance);
  }

  return result;
}

Interval GetInterval(const OBB& obb, const glm::vec3& axis) {
  glm::vec3 vertex[8];

  glm::vec3 C = obb.position;  // OBB Center
  glm::vec3 E = obb.size;  // OBB Extents
  glm::vec3 A[] = {  // OBB Axis
    glm::vec3(obb.orientation[0][0], obb.orientation[0][1], obb.orientation[0][2]),
    glm::vec3(obb.orientation[1][0], obb.orientation[1][1], obb.orientation[1][2]),
    glm::vec3(obb.orientation[2][0], obb.orientation[2][1], obb.orientation[2][2])
  };

  vertex[0] = C + A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
  vertex[1] = C - A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
  vertex[2] = C + A[0] * E[0] - A[1] * E[1] + A[2] * E[2];
  vertex[3] = C + A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
  vertex[4] = C - A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
  vertex[5] = C + A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
  vertex[6] = C - A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
  vertex[7] = C - A[0] * E[0] - A[1] * E[1] + A[2] * E[2];

  Interval result;
  result.min = result.max = glm::dot(axis, vertex[0]);

  for (int i = 1; i < 8; ++i) {
    float projection = glm::dot(axis, vertex[i]);
    result.min = (projection < result.min) ? projection : result.min;
    result.max = (projection > result.max) ? projection : result.max;
  }

  return result;
}

bool SphereSphere(const Sphere& s1, const Sphere& s2) {
  float radiiSum = s1.radius + s2.radius;
  float sqDistance = MagnitudeSq(s1.position - s2.position);
  return sqDistance < radiiSum * radiiSum;
}

void ResetCollisionManifold(CollisionManifold* result) {
  if (result != 0) {
    result->colliding = false;
    result->normal = glm::vec3(0.0f, 0.0f, 1.0f);
    result->depth = FLT_MAX;
    if (result->contacts.size() > 0) {
      result->contacts.clear();
    }
  }
}

std::vector<Point> GetVertices(const OBB& obb) {
  std::vector<glm::vec3> v;
  v.resize(8);

  glm::vec3 C = obb.position;  // OBB Center
  glm::vec3 E = obb.size;  // OBB Extents
  glm::vec3 A[] = {  // OBB Axis
    glm::vec3(obb.orientation[0][0], obb.orientation[0][1], obb.orientation[0][2]),
    glm::vec3(obb.orientation[1][0], obb.orientation[1][1], obb.orientation[1][2]),
    glm::vec3(obb.orientation[2][0], obb.orientation[2][1], obb.orientation[2][2])
  };

  v[0] = C + A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
  v[1] = C - A[0] * E[0] + A[1] * E[1] + A[2] * E[2];
  v[2] = C + A[0] * E[0] - A[1] * E[1] + A[2] * E[2];
  v[3] = C + A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
  v[4] = C - A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
  v[5] = C + A[0] * E[0] - A[1] * E[1] - A[2] * E[2];
  v[6] = C - A[0] * E[0] + A[1] * E[1] - A[2] * E[2];
  v[7] = C - A[0] * E[0] - A[1] * E[1] + A[2] * E[2];

  return v;
}

std::vector<Line> GetEdges(const OBB& obb) {
  std::vector<Line> result;
  result.reserve(12);
  std::vector<Point> v = GetVertices(obb);

  int index[][2] = {  // Indices of edges
    { 6, 1 }, { 6, 3 }, { 6, 4 }, { 2, 7 }, { 2, 5 }, { 2, 0 },
    { 0, 1 }, { 0, 3 }, { 7, 1 }, { 7, 4 }, { 4, 5 }, { 5, 3 }
  };

  for (int j = 0; j < 12; ++j) {
    result.push_back(Line(
      v[index[j][0]], v[index[j][1]]
    ));
  }

  return result;
}

std::vector<Plane> GetPlanes(const OBB& obb) {
  glm::vec3 c = obb.position;  // OBB Center
  glm::vec3 e = obb.size;  // OBB Extents
  glm::vec3 a[] = {  // OBB Axis
    glm::vec3(obb.orientation[0][0], obb.orientation[0][1], obb.orientation[0][2]),
    glm::vec3(obb.orientation[1][0], obb.orientation[1][1], obb.orientation[1][2]),
    glm::vec3(obb.orientation[2][0], obb.orientation[2][1], obb.orientation[2][2])
  };

  std::vector<Plane> result;
  result.resize(6);

  result[0] = Plane(a[0],          glm::dot(a[0], (c + a[0] * e[0])));
  result[1] = Plane(a[0] * -1.0f, -glm::dot(a[0], (c - a[0] * e[0])));
  result[2] = Plane(a[1],          glm::dot(a[1], (c + a[1] * e[1])));
  result[3] = Plane(a[1] * -1.0f, -glm::dot(a[1], (c - a[1] * e[1])));
  result[4] = Plane(a[2],          glm::dot(a[2], (c + a[2] * e[2])));
  result[5] = Plane(a[2] * -1.0f, -glm::dot(a[2], (c - a[2] * e[2])));

  return result;
}

bool ClipToPlane(const Plane& plane, const Line& line, Point* outPoint) {
  glm::vec3 ab = line.end - line.start;

  float nA = glm::dot(plane.normal, line.start);
  float nAB = glm::dot(plane.normal, ab);

  if (CMP(nAB, 0)) return false;

  float t = (plane.distance - nA) / nAB;
  if (t >= 0.0f & t <= 1.0f) {
    if (outPoint != 0) {
      *outPoint = line.start + ab * t;
    }
    return true;
  }

  return false;
}

std::vector<Point> ClipEdgesToOBB(const std::vector<Line>& edges, const OBB& obb) {
  std::vector<Point> result;
  result.reserve(edges.size() * 3);
  Point intersection;

  // std::vector<Plane> plany = GetPlanes(obb);
  // std::vector<Plane>& planes = plany;
  
  const std::vector<Plane>& planes = GetPlanes(obb);  // 여기서 오류나면 위에 두줄로 바꾸기. 여기 발견 가능??

  for (int i = 0; i < planes.size(); ++i) {
    for (int j = 0; j < edges.size(); ++j) {
      if (ClipToPlane(planes[i], edges[j], &intersection)) {
        if (PointInOBB(intersection, obb)) {
          result.push_back(intersection);
        }
      }
    }
  }

  return result;
}

float PenetrationDepth(const OBB& o1, const OBB& o2, const glm::vec3& axis, bool* outShouldFlip) {
  Interval i1 = GetInterval(o1, glm::normalize(axis));
  Interval i2 = GetInterval(o2, glm::normalize(axis));

  if (!((i2.min <= i1.max) && (i1.min <= i2.max))) return 0.0f;  // No penetration

  float len1 = i1.max - i1.min;
  float len2 = i2.max - i2.min;
  float min = fminf(i1.min, i2.min);
  float max = fmaxf(i1.max, i2.max);
  float length = max - min;

  if (outShouldFlip != 0) {
    *outShouldFlip = (i2.min < i1.min);
  }

  return (len1 + len2) - length;
}

CollisionManifold FindCollisionFeatures(const Sphere& A, const Sphere& B) {
  CollisionManifold result;  // Will return result of intersection
  ResetCollisionManifold(&result);

  float r = A.radius + B.radius;
  glm::vec3 d = B.position - A.position;

  if (MagnitudeSq(d) - r * r > 0 || MagnitudeSq(d) == 0.0f) return result;
  d = glm::normalize(d);

  result.colliding = true;
  result.normal = d;
  result.depth = fabsf(MagnitudeSq(d) - r) * 0.5f;

  // dtp - Distance to intersection point
  float dtp = A.radius - result.depth;
  Point contact = A.position + d * dtp;

  result.contacts.push_back(contact);

  return result;
}

CollisionManifold FindCollisionFeatures(const OBB& A, const Sphere& B) {
  CollisionManifold result;
  ResetCollisionManifold(&result);

  Point closestPoint = ClosestPoint(A, B.position);

  float distanceSq = MagnitudeSq(closestPoint - B.position);
  if (distanceSq > B.radius * B.radius) return result;

  glm::vec3 normal;
  if (CMP(distanceSq, 0.0f)) {
    if (CMP(MagnitudeSq(closestPoint - A.position), 0.0f)) return result;

    // Closest point is at the center of the sphere
    normal = glm::normalize(closestPoint - A.position);
  }
  else {
    normal = glm::normalize(B.position - closestPoint);
  }

  Point outsidePoint = B.position - normal * B.radius;

  float distance = Magnitude(closestPoint - outsidePoint);

  result.colliding = true;
  result.contacts.push_back(closestPoint + (outsidePoint - closestPoint) * 0.5f);
  result.normal = normal;
  result.depth = distance * 0.5f;

  return result;
}

CollisionManifold FindCollisionFeatures(const OBB& A, const OBB& B) {
  CollisionManifold result;
  ResetCollisionManifold(&result);

  Sphere s1(A.position, Magnitude(A.size));
  Sphere s2(B.position, Magnitude(B.size));

  if (!SphereSphere(s1, s2)) return result;

  glm::vec3 test[15] = {
    glm::vec3(A.orientation[0][0], A.orientation[0][1], A.orientation[0][2]),
    glm::vec3(A.orientation[1][0], A.orientation[1][1], A.orientation[1][2]),
    glm::vec3(A.orientation[2][0], A.orientation[2][1], A.orientation[2][2]),
    glm::vec3(B.orientation[0][0], B.orientation[0][1], B.orientation[0][2]),
    glm::vec3(B.orientation[1][0], B.orientation[1][1], B.orientation[1][2]),
    glm::vec3(B.orientation[2][0], B.orientation[2][1], B.orientation[2][2])
  };

  for (int i = 0; i < 3; ++i) {
    // Fill out rest of axis
    test[6 + i * 3 + 0] = glm::cross(test[i], test[0]);
    test[6 + i * 3 + 1] = glm::cross(test[i], test[1]);
    test[6 + i * 3 + 2] = glm::cross(test[i], test[2]);
  }

  glm::vec3* hitNormal = 0;
  bool shouldFlip;

  for (int i = 0; i < 15; ++i) {
    if (test[i].x < 0.000001f) test[i].x = 0.0f;
    if (test[i].y < 0.000001f) test[i].y = 0.0f;
    if (test[i].z < 0.000001f) test[i].z = 0.0f;
    if (MagnitudeSq(test[i]) < 0.001f) continue;

    float depth = PenetrationDepth(A, B, test[i], &shouldFlip);
    if (depth <= 0.0f) {
      return result;
    }
    else if (depth < result.depth) {
      if (shouldFlip) {
        test[i] = test[i] * -1.0f;
      }
      result.depth = depth;
      hitNormal = &test[i];
    }
  }

  if (hitNormal == 0) return result;
  glm::vec3 axis = glm::normalize(*hitNormal);

  std::vector<Point> c1 = ClipEdgesToOBB(GetEdges(B), A);
  std::vector<Point> c2 = ClipEdgesToOBB(GetEdges(A), B);
  result.contacts.reserve(c1.size() + c2.size());
  result.contacts.insert(result.contacts.end(), c1.begin(), c1.end());
  result.contacts.insert(result.contacts.end(), c2.begin(), c2.end());

  Interval i = GetInterval(A, axis);
  float distance = (i.max - i.min) * 0.5f - result.depth * 0.5f;
  glm::vec3 pointOnPlane = A.position + axis * distance;

  for (int i = result.contacts.size() - 1; i >= 0; --i) {
    glm::vec3 contact = result.contacts[i];
    result.contacts[i] = contact + (axis * glm::dot(axis, pointOnPlane - contact));

    for (int j = result.contacts.size() - 1; j > i; --j) {
      if (MagnitudeSq(result.contacts[j] - result.contacts[i]) < 0.0001f) {
        result.contacts.erase(result.contacts.begin() + j);
        break;
      }
    }
  }

  result.colliding = true;
  result.normal = axis;

  return result;
}
