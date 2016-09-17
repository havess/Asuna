//
//  Arrow.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "Geometry.hpp"
#include <GL/glew.h>

namespace Asuna{

class Arrow: public Geometry{
public:
  Arrow(vec3 origin, vec3 direction, float length): Geometry(origin), m_direction(direction), m_length(length){}
  void genMesh();

  inline float* getLength(){return &m_length;}
  inline void setLength(const float length){ m_length = length;}

protected:
private:

  vec3  m_direction;
  float m_length;
};
}
