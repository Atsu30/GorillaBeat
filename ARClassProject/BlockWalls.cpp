//
//  BlockWalls.cpp
//  ARClassProject
//
//  Created by Xia Yu on 2019/06/07.
//  Copyright © 2019 森敦史. All rights reserved.
//

#include "BlockWalls.hpp"
#include "DrawPrimitives.h"

void BlockWalls::draw(float* resultTransposedMatrix){
    glLoadIdentity();
    glLoadMatrixf( resultTransposedMatrix);
    //glTranslatef((float) pos.x, (float) pos.y, 0);
    glColor4f(0,0,0,1);
    drawCube(0.01, 0.05, 0.01);
    
    pos.x=resultTransposedMatrix[12];
    pos.y=resultTransposedMatrix[13];
    pos.z=resultTransposedMatrix[14];
    
    std::cout << pos << std:: endl;
    
    length = 0.01;
    height = 0.05;
    width = 0.01;
    
    
}
