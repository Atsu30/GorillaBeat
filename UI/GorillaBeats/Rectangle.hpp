//
//  Rectangle.hpp
//  GorillaBeats
//
//  Created by Cyrus Gao on 28/05/2019.
//  Copyright © 2019 gb. All rights reserved.
//

#ifndef Rectangle_hpp
#define Rectangle_hpp
#define GLFW_INCLUDE_GLU
#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include "Object.h"
#include "Shader.h"

using namespace std;

class Rectangle: public Object {
public:
  Rectangle();
  Rectangle(float *v1, float *size);
  void ready();
  void render();
  
private:
  // Position of four vertices
  float *vertices;
  int *indices;
  void init();
  unsigned int VBO, VAO, EBO;
  Shader sd;
};
#endif /* Rectangle_hpp */
