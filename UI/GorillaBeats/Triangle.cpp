//
//  Triangle.cpp
//  GorillaBeats
//
//  Created by Cyrus Gao on 25/05/2019.
//  Copyright © 2019 gb. All rights reserved.
//

#include "Triangle.hpp"
#include <iostream>


using namespace std;
Triangle::Triangle()
{
    init();
    
}

Triangle::Triangle(float *v1, float *v2, float *v3)
{
    init();
    for (int i =0; i <=2; i++)
    {
        for (int j = 0; j <= 1; j++)
        {
            switch (i) {
                case 0:
                    vertices[i*3+j] = v1[j];
                    break;
                case 1:
                    vertices[i*3+j] = v2[j];
                    break;
                case 2:
                    vertices[i*3+j] = v3[j];
                default:
                    break;
            }
        }
        vertices[i*3+2] = 0.0f;
    }
    
}

void Triangle:: init()
{
    vertices = new float [9]
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    
    sd = Shader("./Shaders/vertex.vs", "./Shaders/fragment.fs");

    
}

void Triangle::ready()
{
    // setting up VAOs and VBOs
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    
}

void Triangle::render()
{
    sd.use();
    // draw our first triangle
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0); // no need to unbind it every time
}

void Triangle::setColour()
{
    for (int i =0; i <= 8; i++) {
        cout<<" "<<vertices[i];
    }
    cout<<endl;
}





