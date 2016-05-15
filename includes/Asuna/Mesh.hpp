//
//  Mesh.hpp
//  Asuna
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include <GL/glew.h>

namespace Asuna{

struct HalfEdge;
struct HE_Vert;
struct HE_Face;

template<typename T>
using sp = sp<T>;
using vec3 = vec3;
using vec2 = glm::vec2;
template<typename T>
using vector = std::vector<T>;

class Vertex{
public:
    Vertex(const vec3& pos, const vec2& texCoord): m_pos(pos), m_textCoord(texCoord){}

    void normalize();

    inline vec3* getPos(){
        return &m_pos;
    }

    inline void setPos(vec3 pos){m_pos =pos;};

    inline vec2* getTextCoord(){
        return &m_textCoord;
    }

protected:
private:
    vec3 m_pos;
    vec2 m_textCoord;
};

struct HalfEdge{
    sp<HalfEdge> next , previous, pair;
    sp<HE_Vert> origin;
    sp<HE_Face> face;
};

struct HE_Vert{
    vec3 position;
    sp<HalfEdge> edge;
};

struct HE_Face{
    vec3 normal;
    sp<HalfEdge> edge;
    long int id;
};

class Mesh{
public:
    Mesh(Vertex* vertices = nullptr, unsigned int numVertices = 0, GLenum drawType = GL_TRIANGLES);
    virtual ~Mesh();
    //void operator =(const Mesh& other){}
    //Mesh(const Mesh& other){}
    inline int getNumVertices(){return m_drawCount;};
    void draw();
protected:
private:
    static const unsigned int NUM_BUFFERS = 2;

    enum{
        POSITION_VB,
        TEXTCOORD_VB
    };

    GLuint m_vertexArrayObject;
    GLuint m_vertexArrayBuffers[NUM_BUFFERS];
    unsigned int m_drawCount;
    GLenum m_drawType;
};
}
