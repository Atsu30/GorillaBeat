//
//  Ball.cpp
//  ARClassProject
//
//  Created by 森敦史 on 2019/05/30.
//  Copyright © 2019 森敦史. All rights reserved.
//

#include "Ball.hpp"

void Ball::move(){
    pos.x = pos.x + vec[0] * dt;
    pos.y = pos.y + vec[1] * dt;
    pos.z = pos.z + vec[2] * dt;
}

void Ball::debug(){
    std::cout << "pos:" << pos << std::endl;
}
