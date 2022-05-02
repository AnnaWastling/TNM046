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
#include "Texture.hpp"
#include "Rotator.hpp"

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

    std::array<GLfloat, 16> matT;
    std::array<GLfloat, 16> matP;
    std::array<GLfloat, 16> matMV;
    std::array<GLfloat, 16> matRx;
    std::array<GLfloat, 16> matRy;
    std::array<GLfloat, 16> matRz;
    std::array<GLfloat, 16> matLightR;

    TriangleSoup myShape;
    TriangleSoup mySphere;
    //myShape.createBox(0.2f, 0.2f, 1.0f);
    myShape.readOBJ("meshes/teapot.obj");
    mySphere.createSphere(1, 100);
    // Generate one texture object with data from a TGA file
    Texture myTexture;
    myTexture.createTexture("textures/earth.tga");

    float time;

    // get shaders
    Shader myShader;

    //create the shaders
    myShader.createShader("vertex.glsl", "fragment.glsl");

    // Locate the sampler2D uniform in the shader program
    GLint locationTex = glGetUniformLocation(myShader.id(), "tex");
    /****************************************************************************/

    glfwSwapInterval(0);  // Do not wait for screen refresh between frames

    GLint locationTime = glGetUniformLocation(myShader.id(), "time");
    if (locationTime == -1) {  // If the variable is not found, -1 is returned
        std::cout << "Unable to locate variable 'time' in shader!\n";
    }

    KeyRotator myKeyRotator(window);
    MouseRotator myMouseRotator(window);

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

        myKeyRotator.poll();
        // Create a rotation matrix that depends on myKeyRotator .phi and myKeyRotator . theta
        myMouseRotator.poll();

        time = static_cast<float>(glfwGetTime());  // Number of seconds since the program was started
        glUniform1f(locationTime, time);        // Copy the value to the shader program

        // Draw the TriangleSoup object mySphere
        // with a shader program that uses a texture
        glBindTexture(GL_TEXTURE_2D, myTexture.id());
        glUniform1i(locationTex, 0);
        //mySphere.render();

        // restore previous state (no texture, no shader)
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);

        //myShape.render();

        // Transformations
        matP = util::mat4identity();
        matMV = util::mat4identity();
        matT = util::mat4identity();

        //translation
        matT = util::mat4translate(0.0f,0.0f,-3.0f);
        matMV = util::mat4mult(matMV, matT); 

        // mouse rotation
        matRx = util::mat4rotx(myMouseRotator.theta());
        matMV = util::mat4mult(matMV, matRx);

        // mouse rotation
        matRz = util::mat4rotx(myMouseRotator.phi());
        matMV = util::mat4mult(matMV, matRz);

        // key rotation
        matRx = util::mat4rotx(myKeyRotator.theta());
        matLightR = util::mat4mult(matLightR, matRx);

        // key rotation
        matRz = util::mat4rotx(myKeyRotator.phi());
        matLightR = util::mat4mult(matLightR, matRz); 

        //projection
        matP = util::mat4perspective(M_PI / 4, 1.0, 0.1, 100.0);

        GLint locationP = glGetUniformLocation(myShader.id(), "P");
        GLint locationMV = glGetUniformLocation(myShader.id(), "MV");
        GLint locationLR = glGetUniformLocation(myShader.id(), "LR");
        glUniformMatrix4fv(locationLR, 1, GL_FALSE, matLightR.data());    // Copy the value
        glUniformMatrix4fv(locationP, 1, GL_FALSE, matP.data());  // Copy the value

      
        // Don´t show when bakside
        //glEnable(GL_CULL_FACE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glCullFace(GL_BACK);
        // 5
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        // copy to shader
        glUniformMatrix4fv(locationMV, 1, GL_FALSE, matMV.data());  // Copy the value


        // Draw the shape
        myShape.render();
       
      // Transformations
       matP = util::mat4identity();
       matMV = util::mat4identity();
       matT = util::mat4identity();

       // translation
       matT = util::mat4translate(0.0f, 0.0f, -3.0f);
       matMV = util::mat4mult(matMV, matT); 

       matRy = util::mat4roty(time*M_PI);
       matMV = util::mat4mult(matMV, matRy);

       glUniformMatrix4fv(locationMV, 1, GL_FALSE, matMV.data());
       mySphere.render();
        // Swap buffers, display the image and prepare for next frame
        glfwSwapBuffers(window);

        // Poll events (read keyboard and mouse input)
        glfwPollEvents();

        // Exit if the ESC key is pressed (and also if the window is closed)
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    // Close the OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}
