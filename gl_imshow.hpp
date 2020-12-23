#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>

#include "glsl_program_wrapper.hpp"

class GL2dImagePanel {
  GLSLProgramWrapper program;

  const size_t img_width, img_height;

  GLuint vao;
  GLuint tbo;
  GLuint eao;

public:
  GL2dImagePanel(size_t img_width, size_t img_height);

  void init();
  void updateImage(const cv::Mat &img);
  void draw();
};

