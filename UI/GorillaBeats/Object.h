//
//  Object.h
//  GorillaBeats
//
//  Created by Cyrus Gao on 27/05/2019.
//  Copyright © 2019 gb. All rights reserved.
//

#ifndef Object_h
#define Object_h
class Object {
public:
  virtual void render() {
    // to be inherited
  }
  virtual void ready() {
    // to be inherited
  }
  void setShaderPath(char* vertexPath, char* fragmentPath)
  {
    this->vertexShaderPath = vertexPath;
    this->fragmentShaderPath = fragmentPath;
  }
  
protected:
  char* vertexShaderPath;
  char* fragmentShaderPath;
  
};

#endif /* Object_h */
