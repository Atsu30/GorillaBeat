//
//  Line.hpp
//  GorillaBeats
//
//  Created by Cyrus Gao on 28/05/2019.
//  Copyright © 2019 gb. All rights reserved.
//

#ifndef Line_hpp
#define Line_hpp
#define GLFW_INCLUDE_GLU
#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include "Triangle.hpp"
#include "Object.h"
#include "Shader.h"

using namespace std;

class Line : public Object {
public:
  Line();
  Line(float *v1, float *v2);
  void ready();
  void render();
  
private:
  // Position of two vertices
  float *vertices;
  void init();
  unsigned int VBO, VAO;
  Shader sd;
};



#endif /* Line_hpp */
