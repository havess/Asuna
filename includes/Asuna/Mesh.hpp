//
//  Mesh.hpp
//  Asuna
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#pragma once

#include "Common.hpp"

//render mask macros here
#define MESH_INDEXED 1

//render mask offsets go here
#define INDEXED_OFFSET 0

#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#endif

namespace Asuna
{

struct HalfEdge;
struct HE_Vert;
struct HE_Face;

struct Vertex
{
public:
  Vertex(const vec3& pos, const vec2& texCoord): m_pos(pos), m_textCoord(texCoord){}
  inline vec3* getPos(){ return &m_pos; }
  inline void setPos(const vec3& pos) { m_pos = pos; }
  inline vec2* getTextCoord(){ return &m_textCoord; }
  inline void setTextCoord(const vec2& textCoord) { m_textCoord = textCoord; }
private:
  vec3 m_pos;
  vec2 m_textCoord;
};

struct HalfEdge
{
    sp<HalfEdge> next, previous, pair;
    sp<HE_Vert> origin;
    sp<HE_Face> face;
};

struct HE_Vert
{
    vec3 position;
    sp<HalfEdge> edge;
};

struct HE_Face
{
    vec3 normal;
    sp<HalfEdge> edge;
    long int id;
};

class Mesh
{
public:
    Mesh(GLenum drawType, long renderMask);
    virtual ~Mesh();
    void addTriangle(unsigned int index1, unsigned int index2, unsigned int index3);
    void addQuad(unsigned int index1, unsigned int index2, unsigned int index3, unsigned int index4);
    void addVertex(Vertex* vert);
    void addIndex(const unsigned int index);
    void mapBuffers();
    void draw();
    void drawIndexed();
    unsigned int getStride();

    inline void setOffset(const vec3& offset) { m_offset = offset; }
    inline int getNumVertices() { return (m_indices.size()) ? m_indices.size():m_vertices.size(); }
    inline Vertex* getVertex(unsigned int index) { return m_vertices[index]; }
    inline unsigned int getIndex(unsigned int index) { return m_indices[index]; }
    inline long getRenderMask() { return m_renderMask; }

private:

    enum{
        POSITION_VB,
        TEXTCOORD_VB,
        INDICES_VB,
        NUM_BUFFERS
    };

    GLuint                  m_vertexArrayObject,
                            m_vertexArrayBuffers[NUM_BUFFERS];
    GLenum                  m_primType;
    vec3                    m_offset = vec3(0,0,0);
    vector<Vertex*>         m_vertices;
    vector<unsigned int>    m_indices;
    long                    m_renderMask;
};
}
