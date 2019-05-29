//
//  Cube.hpp
//  GorillaBeats
//
//  Created by Xia Yu on 2019/05/29.
//  Copyright © 2019 gb. All rights reserved.
//

#ifndef Cube_hpp
#define Cube_hpp

#define GLFW_INCLUDE_GLU
#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"
#include "Shader.h"
#include "Rectangle.hpp"

using namespace std;

class Cube: public Object {
public:
    Cube();
    Cube(float *pos, float *size);
    void ready();
    void render();
    
private:
    float *position;
    float *cubesize;
    float *vertices;
    void init();
    unsigned int VBO, VAO;
    Shader sd;
    
    
};

#endif /* Cube_hpp */
