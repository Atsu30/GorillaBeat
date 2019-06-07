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
    void ready();
    void render();
    void checkdestory();
    
    float radius;
    
    float color;
    
    bool destroy;
    
private:
    float resultMatrix[16];
    void init();
    float startTime;
    float speed;
    int player;
};

#endif /* Ball_hpp */
