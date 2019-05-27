//
//  Window.hpp
//  GorillaBeats
//
//  Created by Cyrus Gao on 25/05/2019.
//  Copyright © 2019 gb. All rights reserved.
//
#define GLFW_INCLUDE_GLU
#define GL_SILENCE_DEPRECATION
#ifndef Window_hpp
#define Window_hpp

#include <iostream>

#include <GLFW/glfw3.h>
#include <vector>
#include "Triangle.hpp"
#include "Object.h"

using std::vector;
class Window
{
public:
    Window(int width, int height, const char *title)
    {
        scr_WIDTH = width;
        scr_HEIGHT = height;
        scr_TITLE = title;
        // preset background colour
        float ary[] = {0.2f, 0.3f, 0.3f, 1.0f};
        bgColour = ary;
    }
    int create(bool isMac)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        if (isMac)
        {
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
        }
        
        // glfw window creation
        // --------------------
        window = glfwCreateWindow(scr_WIDTH, scr_HEIGHT, scr_TITLE, NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        
        return 0;
        
    }
    
    int run();
    void add(Object* obj);
private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow *window);
    GLFWwindow* window;
    float* bgColour;
    
    // settings
    int scr_WIDTH;
    int scr_HEIGHT;
    const char *scr_TITLE;
    vector<Object*> objectVector;

};
#endif /* Window_hpp */
