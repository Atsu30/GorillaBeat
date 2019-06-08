//
//  GameElements.hpp
//  ARClassProject
//
//  Created by Xia Yu on 2019/06/07.
//  Copyright © 2019 森敦史. All rights reserved.
//

#ifndef GameElements_hpp
#define GameElements_hpp

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <opencv2/core.hpp>    // include OpenCV core headers
#include <GLFW/glfw3.h>

#include "Object.hpp"
#include "GameElements.hpp"

class GameElements:public Object{
public:
    
    GameElements();
    GameElements(float startx, float starty, float startz, int T);
    void ready();
    void render();
    void move();
    
    float radius;
    
    float length, height, width;
    
    int type;
    
private:
    float resultMatrix[16];
    void init();
    float startTime;
    float speed;
    
};


#endif /* GameElements_hpp */
