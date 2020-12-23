/**
 * gl_imshow
 *
 * Copyright (c) 2020 kamino410
 *
 * This software is published under the Apache License v2.0.
 * See https://github.com/kamino410/gl_imshow
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "glsl_program_wrapper.hpp"

void keycallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    if (key == GLFW_KEY_ESCAPE) { glfwSetWindowShouldClose(window, GLFW_TRUE); }
  }
}

int main() {
  if (!glfwInit()) exit(1);
  if (atexit(glfwTerminate)) {
    glfwTerminate();
    std::cerr << "Failed to initialize glfw." << std::endl;
    exit(1);
  }

  int num_of_monitors;
  glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
  // GLFWwindow *window = glfwCreateWindow(700, 500, "test", NULL, NULL);
  // -- Display on full screen --
  GLFWmonitor **monitors = glfwGetMonitors(&num_of_monitors);
  GLFWwindow *window = glfwCreateWindow(1920, 1080, "test", monitors[0], NULL);
  if (!window) {
    std::cerr << "Failed to create GL window." << std::endl;
    glfwTerminate();
    exit(1);
  }

  glfwMakeContextCurrent(window);

  std::cout << "Succeeded to create GL window." << std::endl;
  std::cout << "  GL Version   : " << glGetString(GL_VERSION) << std::endl;
  std::cout << "  GL Vendor    : " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "  GL Renderer  : " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "  GLSL Version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << std::endl;

  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize glew." << std::endl;
    glfwTerminate();
    exit(1);
  }

  glfwSwapInterval(0);
  glfwSetKeyCallback(window, keycallback);

  GLSLProgramWrapper program;
  program.compileShader("myshader.vert", GLSLShaderType::VERTEX);
  program.compileShader("myshader.frag", GLSLShaderType::FRAGMENT);
  program.link();

  //  ------------------------------------------------------------
  static const GLfloat vert_data[] = {
      -1.0f, 1.0f,  0.0f,  // top left
      1.0f,  1.0f,  0.0f,  // top right
      -1.0f, -1.0f, 0.0f,  // bottom left
      1.0f,  -1.0f, 0.0f,  // bottom right
  };
  static const GLfloat texcoord_data[] = {
      0.0f, 0.0f,  // top left
      1.0f, 0.0f,  // top right
      0.0f, 1.0f,  // bottom left
      1.0f, 1.0f,  // bottom right
  };
  static const GLuint indexes_data[] = {0, 1, 2, 1, 2, 3};

  // Vertex Array Object
  GLuint vert_buf;
  GLuint texcoord_buf;
  {
    glGenBuffers(1, &vert_buf);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), vert_data, GL_STATIC_DRAW);

    glGenBuffers(1, &texcoord_buf);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buf);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), texcoord_data, GL_STATIC_DRAW);
  }
  GLuint vao;
  {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLint pos_loc = program.getAttribLocation("VertexPosition");
    glEnableVertexAttribArray(pos_loc);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    GLint texcoord_loc = program.getAttribLocation("TextureCoordinate");
    glEnableVertexAttribArray(texcoord_loc);
    glBindBuffer(GL_ARRAY_BUFFER, texcoord_buf);
    glVertexAttribPointer(texcoord_loc, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  GLuint eao;
  {
    glGenBuffers(1, &eao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(GLuint), indexes_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  GLuint tbo;
  cv::Mat img = cv::imread("x-pat.png");
  cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
  {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tbo);
    glBindTexture(GL_TEXTURE_2D, tbo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 img.data);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  int frame = 0;
  std::chrono::system_clock::time_point start, end;
  start = std::chrono::system_clock::now();
  while (!glfwWindowShouldClose(window)) {
    glViewport(0, 0, 1920, 1080);
    glClearColor(0.5, 0.5, 0.5, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    program.enable();

    // 1st attribute buffer : vertices
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
    // glVertexAttribPointer(
    //     0,  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    //     3,  // size
    //     GL_FLOAT,  // type
    //     GL_FALSE,  // normalized?
    //     0,         // stride
    //     (void *)0  // array buffer offset
    // );
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDisableVertexAttribArray(0);

    GLuint tex_id = 0;
    program.setUniform("tex", tex_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 img.data);

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);
    glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();

    frame++;
    if (frame == 200) {
      end = std::chrono::system_clock::now();
      double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
      std::cout << 200 * 1000 / elapsed << std::endl;
      start = std::chrono::system_clock::now();
      frame = 0;
    }
  }

  glfwTerminate();

  return 0;
}
