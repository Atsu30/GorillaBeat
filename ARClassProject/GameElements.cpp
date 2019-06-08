//
//  GameElements.cpp
//  ARClassProject
//
//  Created by Xia Yu on 2019/06/07.
//  Copyright © 2019 森敦史. All rights reserved.
//




#include "GameElements.hpp"
#include <opencv2/core.hpp>    // include OpenCV core headers
#include <GLFW/glfw3.h>
#include "DrawPrimitives.h"




GameElements::GameElements()
{
    init();
}

GameElements::GameElements(float startx, float starty, float startz, int T)
{
    float initMat[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        startx,starty,startz,1
    };
    for (int i = 0;i<16;i++)
    {
        resultMatrix[i] = initMat[i];
    }
    
    length = 0.005;
    height = 0.05;
    width = 0.01;
    type = T;
    radius = 0.01;
    
}

void GameElements::init()
{
    
}

void GameElements::ready()
{
    
}

void GameElements::render()
{
    if (type == 0)
    {
        glLoadIdentity();
        glLoadMatrixf( resultMatrix );
        glColor4f(0.5,0.5,0.5,1);
        drawCube(length, height, width);
    }
    else if (type == 1)
    {
        glLoadIdentity();
        glLoadMatrixf(resultMatrix);
        glColor4f(0,1,0,1);
        drawSphere(radius, 10, 10);
    }
    
    
    GLfloat mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    pos.x =mat[12];
    pos.y =mat[13];
    pos.z =mat[14];
    
 
}

void GameElements::move(){
    if(type == 1){
        resultMatrix[13] += 0.001;
        
        if(resultMatrix[13] > 0.1)resultMatrix[13] = -0.1;
    }
}
