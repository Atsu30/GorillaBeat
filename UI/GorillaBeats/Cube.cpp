//
//  Cube.cpp
//  GorillaBeats
//
//  Created by Xia Yu on 2019/05/29.
//  Copyright © 2019 gb. All rights reserved.
//

#include "Cube.hpp"

using namespace std;

Cube::Cube() { init(); };

Cube::Cube(float *pos, float* size) {
    init();
    position = new float[3]{pos[0], pos[1], pos[2]};
    cubesize = new float[3]{size[0],size[1],size[2]};
    void ready();
    void render();
    
    
    
};

void Cube::init() {
    position = new float[3]{0.0f,0.0f,0.0f};
    cubesize = new float[3]{1.0f,1.0f,1.0f};
    
    vertices = new float[3*6*6]{
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    
    this->setShaderPath("cube.vs", "cube.fs");
    sd = Shader(this->vertexShaderPath, this->fragmentShaderPath);
};

void Cube::ready(){
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*6*6, vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
};

void Cube::render(){
    sd.use();
    
    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    
    view  = glm::translate(view, glm::vec3(1.0f, 0.0f, -3.0f));
    //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(position[0],position[1],position[2]));
    model = glm::scale(model, glm::vec3(cubesize[0],cubesize[1],cubesize[2]));
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    
    
    sd.setMat4("projection", projection);
    sd.setMat4("view", view);
    sd.setMat4("model", model);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
}


