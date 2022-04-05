/*
 * Some utility functions for the course TNM046
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2014
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#include "Utilities.hpp"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>

namespace util {

double displayFPS(GLFWwindow* window) {
    static int frames = 0;
    static double fps = 0.0;

    static double t0 = glfwGetTime();  // Gets number of seconds since glfwInit()

    double t = glfwGetTime();  // Get current time

    // update fps only once every second
    if (t - t0 >= 1.0) {
        fps = static_cast<double>(frames) / (t - t0);
        t0 = t;
        frames = 0;
    }

    // update the window title
    if (frames == 0) {
        char title[201];
        // convert fps to milliseconds
        double frametime = (fps > 0.0) ? 1000.0 / fps : 0.0;
        snprintf(title, 200, "TNM046: %.2f ms/frame (%.1f FPS)", frametime, fps);
        glfwSetWindowTitle(window, title);
    }

    ++frames;
    return fps;
}

GLuint createVertexBuffer(int location, int dimensions, const std::vector<float>& vertices) {
    GLuint bufferID;
    // Generate buffer, activate it and copy the data
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // Tell OpenGL how the data is stored in our buffer
    // Attribute location (must match layout(location=#) statement in shader)
    // Number of dimensions (3 -> vec3 in the shader, 2-> vec2 in the shader),
    // type GL_FLOAT, not normalized, stride 0, start at element 0
    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, 0, nullptr);
    // Enable the attribute in the currently bound VAO
    glEnableVertexAttribArray(location);

    return bufferID;
}

GLuint createIndexBuffer(const std::vector<unsigned int>& indices) {
    GLuint bufferID;
    // Generate buffer, activate it and copy the data
    glGenBuffers(1, &bufferID);
    // Activate (bind) the index buffer and copy data to it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
    // Present our vertex indices to OpenGL
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                 GL_STATIC_DRAW);

    return bufferID;
}
}  // namespace util
