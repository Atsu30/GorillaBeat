//
//  Ball.hpp
//  ARClassProject
//
//  Created by 森敦史 on 2019/05/30.
//  Copyright © 2019 森敦史. All rights reserved.
//

#ifndef Ball_hpp
#define Ball_hpp


#include <iostream>
#include <cstring>
#include "Object.hpp"


class Ball:public Object{
public:
    
    Ball();
    Ball(float *initMatrix, float start, int owner);
    void draw(float* resultTransposedMatrix_world);
    void ready();
    void render();
    
private:
    float resultMatrix[16];
    void init();
    float startTime;
    float speed;
    int player;
    float x,y,z;
};

#endif /* Ball_hpp */
