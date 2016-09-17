//
//  Sphere.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "Geometry.hpp"
#include "../Mesh.hpp"
#include <GL/glew.h>

#define SPHERE_ITERATIONS 3

namespace Asuna{

class Sphere: public Geometry{
public:
  Sphere(vec3 position, float radius): Geometry(position), m_radius(radius){}

  void genMesh();

  inline float* getRadius(){return &m_radius;};
  inline void setRadius(float radius){ m_radius = radius;};

protected:
private:
  unsigned int getMidpoint(unsigned int iA,unsigned int iB);

  map<long int, int> m_middlePointCache;
  float m_radius;
};
}
