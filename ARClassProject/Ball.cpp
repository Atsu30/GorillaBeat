//
//  Ball.cpp
//  ARClassProject
//
//  Created by 森敦史 on 2019/05/30.
//  Copyright © 2019 森敦史. All rights reserved.
//
#include "Ball.hpp"
#define GL_SILENCE_DEPRECATION

#include <opencv2/core.hpp>    // include OpenCV core headers
#include <GLFW/glfw3.h>
#include "DrawPrimitives.h"


Ball::Ball()
{
    init();
}

Ball::Ball(float initMatrix[16], float start, int owner)
{
    for(int i=0;i<16;i++)
    {
        resultMatrix[i]=initMatrix[i];
    }
    startTime = start;
    speed = 0.05f;
    player = owner;
    radius = 0.005;
    color = 1.0;
}

void Ball::init()
{
    
}



void Ball::ready()
{
    
}

void Ball::render()
{
    
    glLoadIdentity();
    glLoadMatrixf(resultMatrix);
//    if (player == 1)
//        glTranslatef(speed*(glfwGetTime()-startTime),0.00,0.0);
//    else if (player ==2)
//        glTranslatef(speed*(glfwGetTime()-startTime),0.00,0.0);
    
    glTranslatef(speed*(glfwGetTime()-startTime),0.00,0.0);
    glColor4f(color,0,0,1);
    drawSphere(radius, 10, 10);
    
    pos.x = resultMatrix[12]+speed*(glfwGetTime()-startTime);
    pos.y = resultMatrix[13];
    pos.z = resultMatrix[14];
    
    //std::cout << pos << std:: endl;
}
