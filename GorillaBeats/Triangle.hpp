//
//  Triangle.hpp
//  GorillaBeats
//
//  Created by Cyrus Gao on 25/05/2019.
//  Copyright © 2019 gb. All rights reserved.
//
#define GLFW_INCLUDE_GLU
#define GL_SILENCE_DEPRECATION
#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>


#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include "Object.h"

using namespace std;

class Triangle: public Object
{
public:
    Triangle();
    Triangle(float *v1, float *v2, float *v3);
    void setColour();
    // Setters and getters for shaders
    void setVertex(const char* tar);
    const char* getVertex();
    void setFragment(const char* tar);
    const char* getFragment();
    void ready();
    void render();
    
    
private:
    // Position of three vertices
    float *v1Pos;
    float *v2Pos;
    float *v3Pos;
    float *vertices;
    const char *vertexShaderSource;
    const char *fragmentShaderSource;
    void init();
    unsigned int VBO, VAO;
    int shaderProgram;

};

#endif /* Triangle_hpp */