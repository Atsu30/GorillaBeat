
#define GLFW_INCLUDE_GLU
#define GL_SILENCE_DEPRECATION

#include <iostream>
#include "Window.hpp"
#include "Triangle.hpp"
#include "Line.hpp"
#include "Rectangle.hpp"
#include "Cube.hpp"
const char* TITLE = "Gorilla Beats";


int main() {
  Window w = Window(800, 600, TITLE);
  if (w.create(true) != -1) {
    float tv1[] = {0.0f,0.0f};
    float tv2[] = {0.5f,0.0f};
    float tv3[] = {0.0f,-0.5f};
    Triangle* d = new Triangle(tv1,tv2,tv3);
    //w.add(d);
    
    float lv1[] = {0.0f, 0.0f};
    float lv2[] = {-0.5f,-0.5f};
    Line *l = new Line(lv1,lv2);
    //w.add(l);
    
    float v1[] = {-0.5f, 0.5f};
    float size[] = {0.5f, 0.5f};
    Rectangle* r = new Rectangle(v1,size);
      
     
    
    //w.add(r);
      
    float pos[] = {-1.0f,-1.0f,0.0f};
    float Csize[] = {0.5f,1.0f,1.0f};
      
      Cube *cube = new Cube(pos, Csize);
      
      w.add(cube);
    
    w.run();
  } else {
    std::cout << "window creation failed " << std::endl;
  }
}

