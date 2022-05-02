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

std::array<float, 16> mat4mult(const std::array<float, 16>& M1, const std::array<float, 16>& M2) {
    std::array<float, 16> Mres;
    // Rad1
    Mres[0] = M1[0] * M2[0] + M1[4] * M2[1] + M1[8] * M2[2] + M1[12] * M2[3];
    Mres[4] = M1[0] * M2[4] + M1[4] * M2[5] + M1[8] * M2[6] + M1[12] * M2[7];
    Mres[8] = M1[0] * M2[8] + M1[4] * M2[9] + M1[8] * M2[10] + M1[12] * M2[11];
    Mres[12] = M1[0] * M2[12] + M1[4] * M2[13] + M1[8] * M2[14] + M1[12] * M2[15];

    // Rad2
    Mres[1] = M1[1] * M2[0] + M1[5] * M2[1] + M1[9] * M2[2] + M1[13] * M2[3];
    Mres[5] = M1[1] * M2[4] + M1[5] * M2[5] + M1[9] * M2[6] + M1[13] * M2[7];
    Mres[9] = M1[1] * M2[8] + M1[5] * M2[9] + M1[9] * M2[10] + M1[13] * M2[11];
    Mres[13] = M1[1] * M2[12] + M1[5] * M2[13] + M1[9] * M2[14] + M1[13] * M2[15];

    // Rad3
    Mres[2] = M1[2] * M2[0] + M1[6] * M2[1] + M1[10] * M2[2] + M1[14] * M2[3];
    Mres[6] = M1[2] * M2[4] + M1[6] * M2[5] + M1[10] * M2[6] + M1[14] * M2[7];
    Mres[10] = M1[2] * M2[8] + M1[6] * M2[9] + M1[10] * M2[10] + M1[14] * M2[11];
    Mres[14] = M1[2] * M2[12] + M1[6] * M2[13] + M1[10] * M2[14] + M1[14] * M2[15];

    // Rad4
    Mres[3] = M1[3] * M2[0] + M1[7] * M2[1] + M1[11] * M2[2] + M1[15] * M2[3];
    Mres[7] = M1[3] * M2[4] + M1[7] * M2[5] + M1[11] * M2[6] + M1[15] * M2[7];
    Mres[11] = M1[3] * M2[8] + M1[7] * M2[9] + M1[11] * M2[10] + M1[15] * M2[11];
    Mres[15] = M1[3] * M2[12] + M1[7] * M2[13] + M1[11] * M2[14] + M1[15] * M2[15];

 return Mres;
}

// Identity matrix = enhetsmatris
std::array<float, 16> mat4identity() {
    std::array<float, 16> res = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

return res;
}

// Print the elements of a matrix M
void mat4print(float M[]) {
    printf(" Matrix :\n");
    printf("%6.2f %6.2f %6.2f %6.2f\n", M[0], M[4], M[8], M[12]);
    printf("%6.2f %6.2f %6.2f %6.2f\n", M[1], M[5], M[9], M[13]);
    printf("%6.2f %6.2f %6.2f %6.2f\n", M[2], M[6], M[10], M[14]);
    printf("%6.2f %6.2f %6.2f %6.2f\n", M[3], M[7], M[11], M[15]);
    printf("\n");
};

std::array<float, 16> mat4rotx( float angle) {
    std::array<float, 16> res = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(angle), sin(angle), 0.0f,
        0.0f, -sin(angle), cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

return res;
}

std::array<float, 16> mat4roty( float angle) {
    std::array<float, 16> res = {
        cos(angle), 0.0f, sin(angle), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin(angle), 0.0f, cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return res;
}
std::array<float, 16> mat4rotz(float angle) {
    std::array<float, 16> res = {
        cos(angle), sin(angle), 0.0f, 0.0f,
        -sin(angle), cos(angle), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return res;
}

std::array<float, 16> mat4scale(float scale) {
    std::array<float, 16> res = {
        scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f, 
        0.0f,  0.0f, 1.0f, 0.0f,
        0.0f, 0.0f,  0.0f, 1.0f
    };

return res;
}

std::array<float, 16> mat4translate(float x, float y, float z) {
    std::array<float, 16> res = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x,    y,    z,    1.0f
    };

    return res;
}
    // vfov is the vertical field of view (in the y direction )
    // aspect is the aspect ratio of the viewport ( width / height )
    // znear is the distance to the near clip plane ( znear > 0)
    // zfar is the distance to the far clip plane ( zfar > znear )
std::array<float, 16> mat4perspective(float vfov, float aspect, float znear, float zfar) {
    float f = 1/tan(vfov/2);
    std::array<float, 16> res = {
        f/aspect, 0.0f, 0.0f, 0.0f ,
        0.0f, f, 0.0f, 0.0f ,
        0.0f, 0.0f, -((zfar+znear)/(zfar-znear)),-1.0f ,
        0.0f, 0.0f, -((2*znear*zfar)/(zfar-znear)), 0.0f
    };

    return res;
}
}  // namespace util
