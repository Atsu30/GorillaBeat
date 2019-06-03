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
#include "Object.hpp"

class Ball : public Object{
public:
    using Object::Object;
    void draw(float* resultTransposedMatrix_world) override;
};

#endif /* Ball_hpp */
