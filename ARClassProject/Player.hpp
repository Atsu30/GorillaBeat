//
//  Player.hpp
//  ARClassProject
//
//  Created by 森敦史 on 2019/06/01.
//  Copyright © 2019 森敦史. All rights reserved.
//
#ifndef Player_hpp
#define Player_hpp

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <opencv2/core.hpp>    // include OpenCV core headers
#include <GLFW/glfw3.h>

#include "Object.hpp"
#include "Ball.hpp"

class Player : public Object{
public:
    using Object::Object;
    Player();
    void draw(float* resultTransposedMatrix_world);
    
    float length, width, height;
    cv::Vec3f color;
    bool state;
};

#endif /* Player_hpp */
