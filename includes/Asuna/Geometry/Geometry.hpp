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
//#include "../../../external/glm/glm.hpp"
#include "../Mesh.hpp"
#include <GL/glew.h>

template<typename T>
using sp = std::shared_ptr<T>;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
template<typename T>
using vector = std::vector<T>;

namespace Asuna{

class Geometry{
public:
    virtual void genMesh() = 0;

    //getters
    inline vec3* getPosition(){return &m_position;};
    inline sp<Mesh> getMesh(){return m_mesh;};
protected:
  Geometry(const vec3& position = vec3(0,0,0)): m_position(position) {}
  //TODO: fix this
  sp<Mesh> m_mesh;
private:
  //inherited but inaccessible variables go here
  vec3 m_position;
};

//For now I will keep all the needed geometry structs here. Might refactor if it starts becoming too large later.
struct TriangleFace{
public:
    //these are the relative indices of the vertices in the vertex list.
    unsigned int v1, v2, v3;
    TriangleFace(unsigned int v1, unsigned int v2, unsigned int v3): v1(v1),  v2(v2), v3(v3) {}
};
}
