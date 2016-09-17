//
//  Mesh.cpp
//  OpenGLTut
//
//  Created by Sam Haves on 2015-12-18.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#include "../includes/Asuna/Mesh.hpp"
#include "../includes/Asuna/Common.hpp"
#include <vector>
#include <iostream>

void CheckOpenGLError(const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("OpenGL error %08x, at %s:%i\n", err, fname, line);
    }
}

namespace Asuna
{

Mesh::Mesh(GLenum drawType, Asuna_Draw_Type renderType) : m_primType(drawType), m_renderMask(renderType)
{
  printf("Generating vertex arrays and %d buffers\n", NUM_BUFFERS);
  glGenVertexArrays(1, &m_vertexArrayObject);
  CheckOpenGLError("Mesh: GenVertexArrays", 20);
  glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
  CheckOpenGLError("Mesh: GenBuffers", 21);
}

Mesh::~Mesh()
{
  printf("Mesh destructor");
  for(auto vert:m_vertices)
  {
    printf("Deleting vertices");
    delete vert;
  }
  glDeleteVertexArraysAPPLE(1,&m_vertexArrayObject);
}

void Mesh::addTriangle(unsigned int index1, unsigned int index2, unsigned int index3)
{
  m_indices.push_back(index1);
  m_indices.push_back(index2);
  m_indices.push_back(index3);
}

void Mesh::addQuad(unsigned int index1, unsigned int index2, unsigned int index3, unsigned int index4)
{
  m_indices.push_back(index1);
  m_indices.push_back(index2);
  m_indices.push_back(index3);
  m_indices.push_back(index4);
}

void Mesh::addVertex(Vertex* vert)
{
  m_vertices.push_back(vert);
}

void Mesh::addIndex(const unsigned int index)
{
  m_indices.push_back(index);
}

void normalize(vec3& v)
{
  double len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
   v.x /= len; v.y /= len; v.z /= len;
}

void Mesh::calculateNormals()
{
  //TODO: add vertex attrib, modify shaders, etc.
  if(m_renderMask & (1 << DRAW_INDEXED)) return calculateNormalsIndexed();
  for(int i = 0; i < getNumVertices(); i += 3)
  {
    vec3 p1 = *m_vertices[i]->getPos(), p2 = *m_vertices[i+1]->getPos(), p3 = *m_vertices[i+2]->getPos();
    //m_normals.push_back(normalize(glm::cross((p2 - p1), (p3-p1))));
  }
}

void Mesh::calculateNormalsIndexed()
{
  //TODO: implement this
  int n = m_renderMask & (1 << DRAW_INDEXED) ? getNumIndices():getNumVertices();
  //for(int i = 0; i < )
}

void Mesh::mapBuffers()
{
  glBindVertexArray(m_vertexArrayObject);
  CheckOpenGLError("Mesh: BindVetexArrayObject", 64);
  vector<vec3> positions;
  vector<vec2> textCoords;

  positions.reserve(m_vertices.size());
  textCoords.reserve(m_vertices.size());

  for(auto vert: m_vertices)
  {
      positions.push_back(*vert->getPos() + m_offset);
      textCoords.push_back(*vert->getTextCoord());
  }

  glBindBuffer(GL_ARRAY_BUFFER , m_vertexArrayBuffers[POSITION_VB]);
  glBufferData(GL_ARRAY_BUFFER, getNumVertices() * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
  CheckOpenGLError("Mesh: glBindVuffer/glBufferData", 78);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  CheckOpenGLError("Mesh: VertexAttribPointer", 83);

  glBindBuffer(GL_ARRAY_BUFFER , m_vertexArrayBuffers[TEXTCOORD_VB]);
  glBufferData(GL_ARRAY_BUFFER, getNumVertices() * sizeof(textCoords[0]), &textCoords[0], GL_STATIC_DRAW);
  CheckOpenGLError("Mesh: glBindVuffer/glBufferData", 85);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
  CheckOpenGLError("Mesh: VertexAttribPointer", 90);

  if(m_renderMask == DRAW_INDEXED)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDICES_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
    CheckOpenGLError("Mesh: glBindBuffer/glBufferData", 107);
  }
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBindVertexArray(0);
}

void Mesh::draw()
{
  glBindVertexArray(m_vertexArrayObject);
  glDrawArrays(m_primType, 0, getNumVertices());
  glBindVertexArray(0);
}

void Mesh::drawIndexed()
{
  glBindVertexArray(m_vertexArrayObject);
  CheckOpenGLError("Mesh: BindVertexArray", 122);
  glDrawElements(m_primType, m_indices.size(), GL_UNSIGNED_INT, (void*) 0);
  CheckOpenGLError("Mesh: glDrawElements", 124);
  glBindVertexArray(0);
}
}
