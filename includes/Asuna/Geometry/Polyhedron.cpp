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

namespace Asuna{

class Polyhedron: public Geometry{
public:
    virtual void genMesh() = 0;
    inline vec3* getPosition(){return &m_position;};
    inline std::shared_ptr<Mesh> getMesh(){return m_mesh;};
protected:
  Geometry(const vec3& position = vec3(0,0,0)): m_position(position) {}
  vec3 m_position;
  //TODO: fix this ugliness
  std::shared_ptr<Mesh> m_mesh;
private:

};

class Octahedron: public Polyhedron{
public:
    Octahedron(const vec3& position, const float height = 20.0f) : Geometry(position), m_height(height), m_len(m_height * cos(0.79)),
    m_vertices(getVertices()){}

    std::vector<Vertex> getVertices(){
        std::vector<Vertex> vertices = {
            Vertex(vec3(-m_len,-m_len,0), vec2(0, 0)),
            Vertex(vec3(-m_len,m_len,0), vec2(0, 0)),
            Vertex(vec3(0,0,m_height), vec2(0, 0)),
            Vertex(vec3(-m_len,-m_len,0), vec2(0, 0)),
            Vertex(vec3(-m_len,m_len,0), vec2(0, 0)),
            Vertex(vec3(0,0,-m_height), vec2(0, 0)),
            Vertex(vec3(m_len,m_len,0), vec2(0, 0)),
            Vertex(vec3(m_len,-m_len,0), vec2(0, 0)),
            Vertex(vec3(0,0,m_height), vec2(0, 0)),
            Vertex(vec3(m_len,m_len,0), vec2(0, 0)),
            Vertex(vec3(m_len,-m_len,0), vec2(0, 0)),
            Vertex(vec3(0,0,-m_height), vec2(0, 0)),
            Vertex(vec3(-m_len,-m_len,0), vec2(0, 0)),
            Vertex(vec3(m_len,-m_len,0), vec2(0, 0)),
            Vertex(vec3(0,0,m_height), vec2(0, 0)),
            Vertex(vec3(-m_len,-m_len,0), vec2(0, 0)),
            Vertex(vec3(m_len,-m_len,0), vec2(0, 0)),
            Vertex(vec3(0,0,-m_height), vec2(0, 0)),
            Vertex(vec3(m_len,m_len,0), vec2(0, 0)),
            Vertex(vec3(-m_len,m_len,0), vec2(0, 0)),
            Vertex(vec3(0,0,m_height), vec2(0, 0)),
            Vertex(vec3(m_len,m_len,0), vec2(0, 0)),
            Vertex(vec3(-m_len,m_len,0), vec2(0, 0)),
            Vertex(vec3(0,0,-m_height), vec2(0, 0)),
        };
        return vertices;
    }

    void genMesh(){
        std::cout << m_vertices.size() << std::endl;
        m_mesh = std::shared_ptr<Mesh>(new Mesh(&m_vertices[0], (int) m_vertices.size() , GL_TRIANGLES));
    }

    float* getHeight() const{return &m_height;};

    void setHeight(const float height){ m_height = height;};
protected:

private:
    unsigned int m_numvertices;
    float m_height, m_len;
    std::vector<Vertex> m_vertices;

};

struct TriangleFace{
public:
    unsigned int v1, v2, v3;

    TriangleFace(unsigned int v1, unsigned int v2, unsigned int v3): v1(v1),  v2(v2), v3(v3) {}
};
}
