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
#include "../Mesh.hpp"
#include <GL/glew.h>

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

namespace Asuna{
class IcoSphereGenerator{
public:
    IcoSphereGenerator(const vec3& position = vec3(0,0,0), const float radius = 10.0f, const int iterations = 10): m_position(position), m_radius(radius), m_iterations(iterations) {}

    void genVertices();

    std::vector<Vertex> getVertices(){return m_vertices;};
    long int getNumVertices(){return m_vertices.size();};
    std::vector<TriangleFace> getIcosahedronFaces();

private:
    std::map<long int, int> m_middlePointCache;
    float m_radius;
    int m_iterations;
    std::vector<Vertex> m_vertices;
    vec3 m_position;

    TriangleFace addFace(const unsigned int index1, const unsigned int index2, const unsigned int index3);

    void normalize(Vertex& vertex, const float length = 1);

    long int getMidpoint(int iA, int iB);
};
class Sphere: public Geometry{
public:
    Sphere(vec3 position, const float radius);

    void genMesh();

   inline float* getRadius(){return &m_radius;};
   inline void setRadius(const float radius){ m_radius = radius;};

protected:
private:
    float m_radius;
    IcoSphereGenerator m_icoGen;
};
}
