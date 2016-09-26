//
//  Mesh.hpp
//  Asuna
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#pragma once

#include "Common.hpp"

enum Asuna_Draw_Type
{
  DRAW_BASIC,
  DRAW_INDEXED
};

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
  ~Vertex(){printf("Vertex is getting destroyed");}
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
    HalfEdge(const sp<HalfEdge> next, const sp<HalfEdge> prev, const sp<HalfEdge> pair, unsigned int origin, const sp<HE_Face> face) : next(next), previous(prev), pair(pair),
      origin(origin), face(face){};
    sp<HalfEdge> next, previous, pair;
    unsigned int origin;
    sp<HE_Face> face;
};

struct HE_Face
{
public:
    HE_Face(const vec3& normal, const sp<HalfEdge> edge) : normal(normal), edge(edge) {}
    vec3 normal;
    sp<HalfEdge> edge;
};

class Mesh
{
public:
    Mesh(GLenum drawType, Asuna_Draw_Type renderMask);
    virtual ~Mesh();
    /* for now these assume that the user passed the proper prim type */
    void addLine(unsigned int index1, unsigned int index2);
    void addTriangle(unsigned int index1, unsigned int index2, unsigned int index3);
    void addQuad(unsigned int index1, unsigned int index2, unsigned int index3, unsigned int index4);
    void addVertex(Vertex* vert);
    void addIndex(unsigned int index);
    void mapBuffers();
    void draw();
    void drawIndexed();
    void calculateNormals();
    unsigned int getStride();

    void setOffset(const vec3& offset) { m_offset = offset;}
    inline void eraseIndices(unsigned int a, unsigned int b) { m_indices.erase(m_indices.begin() + a, m_indices.begin() + b );}
    inline void eraseVertices(unsigned int a, unsigned int b) { m_vertices.erase(m_vertices.begin() + a, m_vertices.begin() + b) ;}
    inline int getNumIndices() { return m_indices.size();}
    inline int getNumVertices() { return m_vertices.size();}
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
    vector<Vertex*>         m_vertices;
    vector<unsigned int>    m_indices;
    vector<vec3>            m_normals;
    Asuna_Draw_Type         m_renderMask;
    vec3                    m_offset;

    void calculateNormalsIndexed();
};
}
