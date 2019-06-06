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
    
    double dt;
    
    //virtual void draw(float* resultTransposedMatrix_world) = 0;
    virtual void render(){
        
    }
    virtual void ready(){
        
    }
    void move();
    void debug();
    
    cv::Point3f pos;
    cv::Vec3f v;
};



#endif /* Object_hpp */
