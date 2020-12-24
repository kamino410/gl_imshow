# gl_imshow

OpenCV's highgui module provides awesome GUI features.
However, it is a little heavy to use for displaying image in fast real-time applications such as VR and AR (~40FPS).

This repository provides an implementation to display an image contained in cv::Mat using OpenGL.

## Requirements

* GLEW
* GLFW3
* OpenCV

## Testing environment

Achieved ~480FPS on the following environment.

* Windows 10
* GTX 1080 Ti
* GLEW 2.2.0
* GLFW 3.3.2
