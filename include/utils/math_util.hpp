#pragma once

#include <glm/glm.hpp>

inline float Magnitude(const glm::vec3& v) {
  return sqrtf(glm::dot(v, v));
}

inline float MagnitudeSq(const glm::vec3& v) {
  return glm::dot(v, v);
}

inline glm::mat3 XRotation3x3(float angle) {
  return glm::mat3(
    1.0f, 0.0f, 0.0f,
    0.0f, cosf(angle), sinf(angle),
    0.0f, -sinf(angle), cosf(angle)
  );
}

inline glm::mat3 YRotation3x3(float angle) {
  return glm::mat3(
    cosf(angle), 0.0f, -sinf(angle),
    0.0f, 1.0f, 0.0f,
    sinf(angle), 0.0f, cosf(angle)
  );
}

inline glm::mat3 ZRotation3x3(float angle) {
  return glm::mat3(
    cosf(angle), sinf(angle), 0.0f,
    -sinf(angle), cosf(angle), 0.0f,
    0.0f, 0.0f, 1.0f
  );
}

inline glm::mat3 Rotation3x3(float pitch, float yaw, float roll) {
  return ZRotation3x3(roll) * XRotation3x3(pitch) * YRotation3x3(yaw);
}

inline glm::vec3 MultiplyVector(const glm::vec3& vec, const glm::mat4& mat) {
  return glm::vec3(
    (vec.x*mat[0][0] + vec.y*mat[1][0] + vec.z*mat[2][0]),
    (vec.x*mat[0][1] + vec.y*mat[1][1] + vec.z*mat[2][1]),
    (vec.z*mat[0][2] + vec.z*mat[1][2] + vec.z*mat[2][2])
  );
}

inline bool AlmostEqualRelativeAndAbs(float A, float B, float maxDiff, float maxRelDiff = FLT_EPSILON) {
  // Check if the numbers are really close.
  // Needed when comparing numbers near zero.
  float diff = fabs(A - B);
  if (diff <= maxDiff) return true;

  A = fabs(A);
  B = fabs(B);
  float largest = (B > A) ? B : A;

  if (diff <= largest * maxRelDiff) {
    return true;
  }
  
  return false;
}

#define CMP(x, y) AlmostEqualRelativeAndAbs(x, y, 0.005f)
