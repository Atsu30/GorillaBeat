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

Ball::Ball(float initMatrix[16], float start)
{
    for(int i=0;i<16;i++)
    {
        resultMatrix[i]=initMatrix[i];
    }
    startTime = start;
    speed = 0.05f;
}

void Ball::init()
{
    
}

void Ball::draw (float* resultTransposedMatrix_world) {
    glLoadIdentity();
    glLoadMatrixf( resultTransposedMatrix_world );
    //glTranslatef((float) pos.x, (float) pos.y, 0);
    glTranslatef(0.02,0.00,0.0);
    glColor4f(1,0,0,1);
    drawSphere(0.005, 10, 10);
}

void Ball::ready()
{
    
}

void Ball::render()
{
    glLoadIdentity();
    glLoadMatrixf(resultMatrix);
    glTranslatef(speed*(glfwGetTime()-startTime),0.00,0.0);
    glColor4f(1,0,0,1);
    drawSphere(0.005, 10, 10);
}
