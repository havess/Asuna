//
//  Plane.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include <vector>
#include <map>
//#include "glm/glm.hpp"
#include "../Mesh.hpp"
#include <GL/glew.h>

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

namespace Asuna{

class Plane: public Geometry{
public:
    Plane(const vec3& position,const vec3& normal = vec3(0,1,0), const float width = 400, const float height = 400):
        Geometry(position), m_normal(normal), m_height(height), m_width(width){}

    virtual void genMesh();

    virtual inline vec3* getNormal(){return &m_normal;};
    virtual inline float* getHeight(){return &m_height;};
    virtual inline float* getWidth(){return &m_width;};

    virtual inline void setNormal(const vec3& normal){ m_normal = normal;};
    virtual inline void setWidth(const float width){ m_width = width;};
    virtual inline void setHeight(const float height){ m_height = height;};

protected:
    vec3 m_normal;
    float m_width, m_height;
private:
};

class Grid: public Plane{
public:
    Grid(const vec3& position,const vec3& normal = vec3(0,1,0), const float width = 1000, const float height = 1000, const bool checkered = false, const float lineSpacing = 10): Plane(position,normal,width,height){
        m_checkered = checkered;
        m_spacing = lineSpacing;
    }

    void genMesh(){
        std::vector<Vertex> vertices;

        for(int i = -m_height/2; i != m_height/2 + m_spacing; i += m_spacing){
            vertices.push_back(Vertex(vec3(-m_width/2, i, 0), vec2(0,0)));
            vertices.push_back(Vertex(vec3(m_width/2, i, 0), vec2(0,0)));
            vertices.push_back(Vertex(vec3(i, -m_width/2, 0), vec2(0,0)));
            vertices.push_back(Vertex(vec3(i, m_width/2, 0), vec2(0,0)));
        }

        Vertex* verts = &vertices[0];
        m_mesh = std::make_shared<Mesh>(Mesh(GL_TRIANGLES, DRAW_BASIC));
    }

protected:
private:
    bool m_checkered;
    float m_spacing;
};
}
