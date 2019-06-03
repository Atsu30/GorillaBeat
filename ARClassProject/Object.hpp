//
//  Object.hpp
//  ARClassProject
//
//  Created by 森敦史 on 2019/06/01.
//  Copyright © 2019 森敦史. All rights reserved.
//
#ifndef Object_hpp
#define Object_hpp


#include <iostream>
#include <opencv2/core.hpp>    // include OpenCV core headers

class Object{
public:
    cv::Point3f pos;
    cv::Vec3f v;
    double dt;
    
    Object(cv::Point3f startPos, cv::Vec3f startv);
    void setPos(cv::Point3f new_pos);
    void setVec(cv::Vec3f new_v);
    virtual void draw(float* resultTransposedMatrix_world) = 0;
    void move();
    void debug();
};



#endif /* Object_hpp */
