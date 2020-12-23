/**
 * gl_imshow
 *
 * Copyright (c) 2020 kamino410
 *
 * This software is published under the Apache License v2.0.
 * See https://github.com/kamino410/gl_imshow
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

enum class GLSLShaderType {
  VERTEX = GL_VERTEX_SHADER,
  FRAGMENT = GL_FRAGMENT_SHADER,
  GEOMETRY = GL_GEOMETRY_SHADER,
  TESS_CONTROL = GL_TESS_CONTROL_SHADER,
  TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
  COMPUTE = GL_COMPUTE_SHADER
};

class GLSLProgramWrapper {
private:
  GLuint handle;

  std::string loadFileAsStr(const char *);

public:
  GLSLProgramWrapper();
  ~GLSLProgramWrapper();

  void compileShader(const char *, GLSLShaderType);

  void link();

  void bindAttribLocation(GLuint, const char *);
  GLint getAttribLocation(const char *);

  void setUniform(const char *, GLuint);
  void setUniform(const char *, int);
  void setUniform(const char *, bool);
  void setUniform(const char *, float);

  void enable();
};
