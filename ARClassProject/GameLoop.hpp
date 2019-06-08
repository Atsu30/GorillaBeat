//
//  GameLoop.hpp
//  ARClassProject
//
//  Created by Cyrus Gao on 08/06/2019.
//  Copyright © 2019 森敦史. All rights reserved.
//

#ifndef GameLoop_hpp
#define GameLoop_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>
float liveMat[] =
{
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    -0.09,0.05,-0.2,1
};
void updateLives(int p1, int p2)
{
    glColor4f(0, 1, 0, 1);
    glLoadIdentity();
    glLoadMatrixf(liveMat);

    do {
        drawRectangle(0.008, 0.008);
        glTranslatef(0.01, 0, 0);
        p1--;
    } while (p1 > 0);

    glTranslatef(0.14, 0, 0);
    do {
        drawRectangle(0.008, 0.008);
        glTranslatef(-0.01, 0, 0);
        p2--;
    } while (p2 > 0);
}
#endif /* GameLoop_hpp */
