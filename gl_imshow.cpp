#include "gl_imshow.hpp"

#include <iostream>

void (size_t img_width, size_t img_height)
  :img_width(img_width), img_height(img_height) {}

void GL2dImagePanel::init() {
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
  {
    glGenBuffers(1, &eao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(GLuint), indexes_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

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
}

void GL2dImagePanel::updateImage(const cv::Mat &img) {
  if (img.cols != img_width || img.rows != img_height) {
    std::cerr << "Invalid image size!" << std::endl;
    return;
  }

  glBindTexture(GL_TEXTURE_2D, tbo);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
               img.data);
}

void GL2dImagePanel::draw() {
  program.enable();

  GLuint tex_id = 0;
  program.setUniform("tex", tex_id);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);
  glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);
}

