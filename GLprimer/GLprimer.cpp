/*
 * A C++ framework for OpenGL programming in TNM046 for MT1
 *
 * This is a small, limited framework, designed to be easy to use for students in an introductory
 * computer graphics course in the first year of a M Sc curriculum. It uses custom code for some
 * things that are better solved by external libraries like GLM, but the emphasis is on simplicity
 * andreadability, not generality.
 *
 * For the window management, GLFW 3.x is used for convenience.
 * The framework should work in Windows, MacOS X and Linux.
 * GLEW is used for handling platform specific OpenGL extensions.
 * This code depends only on GLFW, GLEW, and OpenGL libraries.
 * OpenGL 3.3 or higher is required.
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2013-2015
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#if defined(WIN32) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

// File and console I/O for logging and error reporting
#include <iostream>
// Math header for trigonometric functions
#include <cmath>

// glew provides easy access to advanced OpenGL functions and extensions
#include <GL/glew.h>

// GLFW 3.x, to handle the OpenGL window
#include <GLFW/glfw3.h>


#include "Utilities.hpp"
#include <vector>
#include <array>
#include "Shader.hpp"
#include "TriangleSoup.hpp"

/*
 * main(int argc, char* argv[]) - the standard C++ entry point for the program
 */
int main(int, char*[]) {
    // Initialise GLFW
    glfwInit();

    const GLFWvidmode* vidmode;  // GLFW struct to hold information about the display
    // Determine the desktop size
    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // Make sure we are getting a GL context of at least version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable the OpenGL core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Open a square window (aspect 1:1) to fill half the screen height
    GLFWwindow* window =
        glfwCreateWindow(vidmode->height / 2, vidmode->height / 2, "GLprimer", nullptr, nullptr);
    if (!window) {
        std::cout << "Unable to open window. Terminating.\n";
        glfwTerminate();  // No window was opened, so we can't continue in any useful way
        return -1;
    }

    // Make the newly created window the "current context" for OpenGL
    // (This step is strictly required or things will simply not work)
    glfwMakeContextCurrent(window);

    // Initialize glew
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << "\n";
        glfwTerminate();
        return -1;
    }

    // Show some useful information on the GL context
    std::cout << "GL vendor:       " << glGetString(GL_VENDOR)
              << "\nGL renderer:     " << glGetString(GL_RENDERER)
              << "\nGL version:      " << glGetString(GL_VERSION)
              << "\nDesktop size:    " << vidmode->width << " x " << vidmode->height << "\n";

    /****************Declarations****************************/
    // for resizing window
    int width, height;

    // // colorarray
    //const std::vector<GLfloat> colorArrayData = {
    //    // P0
    //    1.0f, 0.0f, 0.3f,  // Red
    //    0.0f, 0.5f, 0.0f,  // Green
    //    0.3f, 0.3f, 0.3f,  // Blue
    //    // P1
    //    1.0f, 0.0f, 0.3f,  // Red
    //    0.2f, 0.2f, 0.2f,  // Green
    //    0.3f, 0.3f, 0.3f,  // Blue
    //    // P2
    //    1.0f, 0.0f, 0.3f,  // Red
    //    0.2f, 0.2f, 0.2f,  // Green
    //    0.0f, 0.0f, 0.0f,  // Blue
    //    // P3
    //    1.0f, 0.0f, 0.3f,  // Red
    //    0.0f, 0.5f, 0.0f,  // Green
    //    0.0f, 0.0f, 0.0f,  // Blue

    //    // P4
    //    0.8f, 0.8f, 0.8f,  // Red
    //    0.0f, 0.5f, 0.0f,  // Green
    //    0.3f, 0.3f, 0.3f,  // Blue

    //    // P5
    //    0.8f, 0.8f, 0.8f,  // Red
    //    0.2f, 0.2f, 0.2f,  // Green
    //    0.3f, 0.3f, 0.3f,  // Blue

    //    // P6
    //    0.8f, 0.8f, 0.8f,  // Red
    //    0.2f, 0.2f, 0.2f,  // Green
    //    0.0f, 0.0f, 0.0f,  // Blue

    //    // P7
    //    0.8f, 0.8f, 0.8f,  // Red
    //    0.0f, 0.5f, 0.0f,  // Green
    //    0.0f, 0.0f, 0.0f,  // Blue
    //}; 
     std::array<GLfloat, 16> matT;
    std::array<GLfloat, 16> matR;
    std::array<GLfloat, 16> matRes;

    float time;
    // get shaders
    Shader myShader;

    //create the shaders
    myShader.createShader("vertex.glsl", "fragment.glsl");
    /****************************************************************************/

    glfwSwapInterval(0);  // Do not wait for screen refresh between frames

    GLint locationTime = glGetUniformLocation(myShader.id(), "time");
    if (locationTime == -1) {  // If the variable is not found, -1 is returned
        std::cout << "Unable to locate variable 'time' in shader!\n";
    }
    TriangleSoup myShape;
    myShape.createSphere(0.5,100);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        /******************lab1*************************/
        // Get window size. It may start out different from the requested size and
        // will change if the user resizes the window
        glfwGetWindowSize(window, &width, &height);
        // Set viewport. This is the pixel rectangle we want to draw into
        glViewport(0, 0, width, height);  // The entire window
        /******************lab1*************************/
        //display frame rate
        util::displayFPS(window);

        // Set the clear color to a dark gray (RGBA)
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        // Clear the color and depth buffers for drawing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(myShader.id());  // Activate the shader to set its variables

        /* ---- Rendering code should go here ---- */     
        time = static_cast<float>(glfwGetTime());  // Number of seconds since the program was started
        glUniform1f(locationTime, time);        // Copy the value to the shader program

        myShape.render();

        // Transformations
        matT = util::mat4identity();
        matT = util::mat4translate(0.1, 0.1, 0.0);
        matR = util::mat4identity();
        matR = util::mat4roty(time  * M_PI);
        matRes = util::mat4mult(matT, matR);

        GLint locationT = glGetUniformLocation(myShader.id(), "T");
        GLint locationR = glGetUniformLocation(myShader.id(), "R");
        GLint locationRes = glGetUniformLocation(myShader.id(), "Res");


        glUniformMatrix4fv(locationT, 1, GL_FALSE, matT.data());  // Copy the value
        glUniformMatrix4fv(locationR, 1, GL_FALSE, matR.data());  // Copy the value
        glUniformMatrix4fv(locationRes, 1, GL_FALSE, matRes.data());  // Copy the value
      
        // Don´t show when bakside
        //glEnable(GL_CULL_FACE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glCullFace(GL_BACK);

        // Swap buffers, display the image and prepare for next frame
        glfwSwapBuffers(window);

        // Poll events (read keyboard and mouse input)
        glfwPollEvents();

        // Exit if the ESC key is pressed (and also if the window is closed)
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
    /***********************lab1******************************/
    // release the vertex and index buffers as well as the vertex array
   // glDeleteBuffers(1, &colorBufferID);

    // Close the OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}
