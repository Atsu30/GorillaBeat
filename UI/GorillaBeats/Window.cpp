//
//  Window.cpp
//  GorillaBeats
//
//  Created by Cyrus Gao on 25/05/2019.
//  Copyright © 2019 gb. All rights reserved.
//

#include "Window.hpp"

int Window::run() {
  // safely convert long to int without warning
  int size = static_cast<int>(this->objectVector.size());
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < size; i++) {
      objectVector.at(i)->render();
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width,
                                       int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void Window::processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void Window::add(Object* obj) {
  obj->ready();
  objectVector.push_back(obj);
}

