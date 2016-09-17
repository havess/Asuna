//
//  Cone.hpp
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

class Cone: public Geometry{
public:
  Cone(vec3 pos, vec3 direction, float height, float width): Geometry(pos), m_direction(direction), m_height(height), m_width(width){}
  void genMesh();

  inline float* getHeight(){return &m_height;}
  inline void setHeight(float height){ m_height = height;}

protected:
private:
  vec3  m_direction;
  float m_height, m_width;
};
}
