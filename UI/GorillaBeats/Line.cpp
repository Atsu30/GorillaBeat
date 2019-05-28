//
//  Line.cpp
//  GorillaBeats
//
//  Created by Cyrus Gao on 28/05/2019.
//  Copyright © 2019 gb. All rights reserved.
//

#include "Line.hpp"

Line::Line() { init(); }

Line::Line(float *v1, float *v2) {
  init();
  vertices[0] = v1[0];
  vertices[1] = v2[1];
}
void Line::ready()
{
  
}
void Line::render()
{
  
}

void Line::init() { vertices = new float[4]{0.0f, 0.0f, 0.0f, 0.5f}; }
