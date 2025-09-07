#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glad/glad.h"

static std::string load_text_file(const std::string &path) {
  std::ifstream ifs(path, std::ios::binary);
  if (!ifs) throw std::runtime_error("Failed to open: " + path);

  std::ostringstream ss;
  ss << ifs.rdbuf();

  return ss.str();
}

static GLuint compile(GLenum type, const char* src) {
  GLuint s = glCreateShader(type);
  glShaderSource(s, 1, &src, nullptr);
  glCompileShader(s);

  GLint success;
  glGetShaderiv(s, GL_COMPILE_STATUS, &success);
  if (!success) {
    char log[1024];
    glGetShaderInfoLog(s, 1024, nullptr, log);
    std::cerr << "Shader compile error: " << log << std::endl;
    std::exit(1);
  }

  return s;
}

static GLuint link(GLuint vs, GLuint fs) {
  GLuint p = glCreateProgram();
  glAttachShader(p, vs);
  glAttachShader(p, fs);
  glLinkProgram(p);

  GLint success;
  glGetProgramiv(p, GL_LINK_STATUS, &success);
  if (!success) {
    char log[1024];
    glGetProgramInfoLog(p, 1024, nullptr, log);
    std::cerr << "Program link error: " << log << std::endl;
    std::exit(1);
  }

  glDetachShader(p, vs);
  glDetachShader(p, fs);
  glDeleteShader(vs);
  glDeleteShader(fs);

  return p;
}
