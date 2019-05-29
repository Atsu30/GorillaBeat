//
//  Rectangle.cpp
//  GorillaBeats
//
//  Created by Cyrus Gao on 28/05/2019.
//  Copyright © 2019 gb. All rights reserved.
//

#include "Rectangle.hpp"
using namespace std;
Rectangle::Rectangle() { init(); }

Rectangle::Rectangle(float *v1, float* size) {
  init();
  float length = size[0];
  float height = size[1];
  vertices = new float [12]{
    v1[0] + length, v1[1], 0.0f,
    v1[0] + length, v1[1] - length, 0.0f,
    v1[0], v1[1] - height, 0.0f,
    v1[0], v1[1],0.0f,
  };
}

void Rectangle::init() {
  vertices =
  new float[12]{
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
  };
  indices = new int[6] {
    0,1,3,
    1,2,3
  };
  
  this->setShaderPath("vertex.vs", "fragment.fs");
  sd = Shader(this->vertexShaderPath, this->fragmentShaderPath);
}

void Rectangle::ready() {
  // setting up VAOs and VBOs
  
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 12, indices, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  
  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't
  // unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}

void Rectangle::render() {
  sd.use();
  // draw our first triangle
  glBindVertexArray(VAO);  // seeing as we only have a single VAO there's no need
  // to bind it every time, but we'll do so to keep
  // things a bit more organized
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);  // no need to unbind it every time
}

