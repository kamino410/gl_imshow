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

void keycallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    if (key == GLFW_KEY_ESCAPE) { glfwSetWindowShouldClose(window, GLFW_TRUE); }
  }
}

int main() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize glfw." << std::endl;
    exit(1);
  }
  if (atexit(glfwTerminate)) {
    glfwTerminate();
    std::cerr << "Failed to set atexit(glfwTerminate)." << std::endl;
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

  cv::Mat img = cv::imread("x-pat.png");
  cv::cvtColor(img, img, cv::COLOR_BGR2RGB);

  int frame = 0;
  std::chrono::system_clock::time_point start, end;
  start = std::chrono::system_clock::now();
  while (!glfwWindowShouldClose(window)) {
    glViewport(0, 0, 1920, 1080);
    glClearColor(0.5, 0.5, 0.5, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    

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

  return 0;
}

