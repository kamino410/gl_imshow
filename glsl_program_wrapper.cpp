/**
 * gl_imshow
 *
 * Copyright (c) 2020 kamino410
 *
 * This software is published under the Apache License v2.0.
 * See https://github.com/kamino410/gl_imshow
 */

#include "glsl_program_wrapper.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

GLSLProgramWrapper::GLSLProgramWrapper() { handle = glCreateProgram(); }

GLSLProgramWrapper::~GLSLProgramWrapper() {}

std::string GLSLProgramWrapper::loadFileAsStr(const char* filename) {
  std::ifstream fs(filename, std::ios::in);
  if (!fs.is_open()) {
    std::cerr << "Failed to open file : " << filename << std::endl;
    exit(1);
  }
  std::stringstream ss;
  ss << fs.rdbuf();
  fs.close();
  return ss.str();
}

void GLSLProgramWrapper::compileShader(const char* filename, GLSLShaderType type) {
  std::string str = loadFileAsStr(filename);
  const char* code = str.c_str();

  const GLuint sHandle = glCreateShader((GLenum)type);
  if (!sHandle) {
    std::cerr << "Failed to create shader : " << filename << std::endl;
    exit(1);
  }

  glShaderSource(sHandle, 1, &code, NULL);
  glCompileShader(sHandle);

  GLint status;
  glGetShaderiv(sHandle, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    std::cerr << "Shader Compile Error in " << filename << std::endl;

    GLsizei bufSize;
    glGetShaderiv(sHandle, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 0) {
      std::vector<GLchar> infoLog(bufSize);
      GLsizei length;
      glGetShaderInfoLog(sHandle, bufSize, &length, &infoLog[0]);
      std::cerr << &infoLog[0] << std::endl;
    }
    glDeleteShader(sHandle);
    exit(1);
  }

  glAttachShader(handle, sHandle);
}

void GLSLProgramWrapper::link() {
  glLinkProgram(handle);

  GLint status;
  glGetProgramiv(handle, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    std::cerr << "Link Error." << std::endl;

    GLsizei bufSize;
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 1) {
      std::vector<GLchar> infoLog(bufSize);
      GLsizei length;
      glGetProgramInfoLog(handle, bufSize, &length, &infoLog[0]);
      std::cerr << &infoLog[0] << std::endl;
    }
    glDeleteProgram(handle);
    exit(1);
  }
}

void GLSLProgramWrapper::bindAttribLocation(GLuint location, const char* attrname) {
  glBindAttribLocation(handle, location, attrname);
}
GLint GLSLProgramWrapper::getAttribLocation(const char* attrname) {
  return glGetAttribLocation(handle, attrname);
}

void GLSLProgramWrapper::setUniform(const char* name, float val) {
  GLint loc = glGetUniformLocation(handle, name);
  glUniform1f(loc, val);
}
void GLSLProgramWrapper::setUniform(const char* name, int val) {
  GLint loc = glGetUniformLocation(handle, name);
  glUniform1i(loc, val);
}
void GLSLProgramWrapper::setUniform(const char* name, GLuint val) {
  GLint loc = glGetUniformLocation(handle, name);
  glUniform1ui(loc, val);
}
void GLSLProgramWrapper::setUniform(const char* name, bool val) {
  int loc = glGetUniformLocation(handle, name);
  glUniform1i(loc, val);
}

void GLSLProgramWrapper::enable() { glUseProgram(handle); }
