
#define GLFW_INCLUDE_GLU
#define GL_SILENCE_DEPRECATION


#include <iostream>
#include "Window.hpp"
#include "Triangle.hpp"
const char *TITLE = "Gorilla Beats";

int main()
{
    Window w = Window(800, 600, TITLE);
    if (w.create(true) != -1)
    {
        Triangle* c = new Triangle;
        w.add(c);
        w.run();
    }
    else
    {
        std::cout<<"window creation failed "<<std::endl;
    }
    
}


