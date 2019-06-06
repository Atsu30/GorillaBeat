//
//  Player.cpp
//  ARClassProject
//
//  Created by 森敦史 on 2019/06/01.
//  Copyright © 2019 森敦史. All rights reserved.
//

#include "Player.hpp"
#include "DrawPrimitives.h"

void Player::draw(float* resultTransposedMatrix_world){
    glLoadIdentity();
    glLoadMatrixf( resultTransposedMatrix_world );
    glTranslatef((float) pos.x, (float) pos.y, 0);
    glColor4f(0,0,0,1);
    drawCube(0.01, 0.05, 0.01);
    
    pos.x=resultTransposedMatrix_world[12];
    pos.y=resultTransposedMatrix_world[13];
    pos.z=resultTransposedMatrix_world[14];
    
    //std::cout << pos << std:: endl;
    
    length = 0.01;
    height = 0.05;
    width = 0.01;
    
    
}
//
//void shootBall(Player player, std::vector<Object>& objects){
//    Ball ball(player.pos, cv::Vec3f(1.0, 0, 0)); // make ball instance and initialize at vec(1,0,0)
//    objects.push_back(ball); // register ball
//}
