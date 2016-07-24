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

#define SPHERE_ITERATIONS 5

namespace Asuna{

class Sphere: public Geometry{
public:
  Sphere::Sphere(vec3 position, const float radius): Geometry(position), m_radius(radius){}

  void genMesh();

  inline float* getRadius(){return &m_radius;};
  inline void setRadius(const float radius){ m_radius = radius;};

protected:
private:
  void normalize(Vertex& vertex, const float length = 1);
  unsigned int getMidpoint(const unsigned int iA,const unsigned int iB);

  map<long int, int> m_middlePointCache;
  float m_radius;
};
}
