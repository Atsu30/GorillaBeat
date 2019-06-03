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

void Ball::draw (float* resultTransposedMatrix_world) {
    glLoadIdentity();
    glLoadMatrixf( resultTransposedMatrix_world );
    glTranslatef((float) pos.x, (float) pos.y, 0);
    glColor4f(1,0,0,1);
    drawSphere(0.005, 10, 10);
}
