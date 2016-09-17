//
//  Geometry.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

/*
This is the geometry *super* class. All primitives will be derived from this class.

******Each child needs to implement genMesh(). This function should assigned a shared pointer to an instance of its Mesh*****
*/

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "../Common.hpp"
#include "../Mesh.hpp"
#include <GL/glew.h>

namespace Asuna{

class Geometry{
public:
  void render();

  //getters
  inline vec3* getPosition() { return &m_position; }
  inline sp<Mesh> getMesh() { return m_mesh; }

protected:
  Geometry(const vec3 position): m_position(position) {}
  virtual void genMesh() = 0;
  void normalize(Vertex* vertex, float length);
  sp<Mesh> m_mesh;
  vec3 m_position;
private:
};

}
