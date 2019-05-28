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
  vertices[1] = v1[1];
  vertices[2] = v2[0];
  vertices[3] = v2[1];
}
void Line::ready() {
  // setting up VAOs and VBOs

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
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
  std::cout<<vertices[0]<<std::endl;
  std::cout<<vertices[1]<<std::endl;
  std::cout<<vertices[2]<<std::endl;
  std::cout<<vertices[3]<<std::endl;
}
void Line::render() {
  sd.use();
  // draw our first triangle
  glBindVertexArray(
      VAO);  // seeing as we only have a single VAO there's no need
  // to bind it every time, but we'll do so to keep
  // things a bit more organized
  glDrawArrays(GL_LINES, 0, 2);
  glBindVertexArray(0);  // no need to unbind it every time
}

void Line::init() {
  vertices = new float[4]{0.0f, 0.0f, 0.0f, 0.5f};
  this->setShaderPath("vertex.vs", "fragment.fs");
  sd = Shader(this->vertexShaderPath, this->fragmentShaderPath);
}
