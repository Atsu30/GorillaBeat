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
#include <opencv2/core.hpp>    // include OpenCV core headers
#include <GLFW/glfw3.h>

class Ball{
public:
    cv::Point3f pos;
    cv::Vec3f vec;
    double dt;
    
    Ball(cv::Point3d startPos){
        pos = startPos;
        vec = cv::Vec3d(1, 1, 1);
        dt = 0.001;
    }
    
    void setPos(cv::Point3f new_pos){
        pos = new_pos;
    }
    
    void setVec(cv::Vec3f new_vec){
        vec = new_vec;
    }
    void move();
    void debug();
};

#endif /* Ball_hpp */
