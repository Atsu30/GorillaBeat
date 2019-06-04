//
//  Object.cpp
//  ARClassProject
//
//  Created by 森敦史 on 2019/06/01.
//  Copyright © 2019 森敦史. All rights reserved.
//

#include "Object.hpp"
#include <opencv2/core.hpp>    // include OpenCV core headers


void Object::setPos(cv::Point3f new_pos){
    pos = new_pos;
}

void Object::setVec(cv::Vec3f new_v){
    v = new_v;
}

void Object::move(){
    pos.x = pos.x + v[0] * dt;
    pos.y = pos.y + v[1] * dt;
    pos.z = pos.z + v[2] * dt;
}

//void Object::draw(float* resultTransposedMatrix_world){
//    glLoadIdentity();
//    glLoadMatrixf( resultTransposedMatrix_world );
//    glTranslatef((float) pos.x, (float) pos.y, 0);
//    glColor4f(1,0,0,1);
//    drawSphere(0.005, 10, 10);
//}



void Object::debug(){
    std::cout << "pos:" << pos << std::endl;
    std::cout << "vlocity:" << v << std::endl;
}
