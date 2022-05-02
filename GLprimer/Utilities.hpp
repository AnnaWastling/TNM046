/*
 * Some utility functions for the course TNM046
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2013-2014
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#pragma once
// glew provides easy access to advanced OpenGL functions and extensions
#include <GL/glew.h>

// GLFW 3.x, to handle the OpenGL window
#include <GLFW/glfw3.h>
#include <vector>
#include <array>


struct GLFWwindow;

namespace util {

/*
 * displayFPS() - Calculate, display and return frame rate statistics.
 * Called every frame, but statistics are updated only once per second.
 * The time per frame is a better measure of performance than the
 * number of frames per second, so both are displayed.
 *
 * NOTE: This function doesn't work properly if you call it for multiple
 * windows. Call it only for one window, and only once every frame.
 */
double displayFPS(GLFWwindow* window);
/********lab1********/
GLuint createVertexBuffer(int location, int dimensions, const std::vector<float>& vertices);
GLuint createIndexBuffer(const std::vector<unsigned int>& indices);
/*******lab2**********/
std::array<float, 16> mat4mult(const std::array<float, 16>& M1, const std::array<float, 16>& M2);
std::array<float, 16> mat4identity();
void mat4print(float M[]);
std::array<float, 16> mat4rotx(float angle);
std::array<float, 16> mat4roty(float angle);
std::array<float, 16> mat4rotz(float angle);
std::array<float, 16> mat4scale(float scale);
std::array<float, 16> mat4translate(float x, float y, float z);
std::array<float, 16> mat4perspective(float vfov, float aspect, float znear, float zfar);

}  // namespace util
