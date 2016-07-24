//
//  Mesh.cpp
//  OpenGLTut
//
//  Created by Sam Haves on 2015-12-18.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#include "../includes/Asuna/Mesh.hpp"
#include <vector>
#include <iostream>

namespace Asuna
{

Mesh::Mesh(GLenum drawType, long renderMask) : m_primType(drawType), m_renderMask(renderMask)
{
  glGenVertexArrays(1, &m_vertexArrayObject);
  glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
}

Mesh::~Mesh()
{
  for(auto vert:m_vertices)
  {
    delete vert;
  }
  glDeleteVertexArraysAPPLE(1,&m_vertexArrayObject);
}

//TODO: change to unsigned int
void Mesh::addTriangle(unsigned int index1, uint index2, uint index3)
{
  m_indices.push_back(index1);
  m_indices.push_back(index2);
  m_indices.push_back(index3);
}

void Mesh::addQuad(uint index1, uint index2, uint index3, uint index4)
{
  m_indices.push_back(index1);
  m_indices.push_back(index2);
  m_indices.push_back(index3);
  m_indices.push_back(index4);
}

void Mesh::addVertex(Vertex* vert)
{
  *vert->getPos() += m_offset;
  m_vertices.push_back(vert);
}

void Mesh::addIndex(const unsigned int index)
{
  m_indices.push_back(index);
}

void Mesh::mapBuffers()
{
  glBindVertexArray(m_vertexArrayObject);

  vector<vec3> positions;
  vector<vec2> textCoords;

  positions.reserve(m_vertices.size());
  textCoords.reserve(m_vertices.size());

  for(auto vert: m_vertices)
  {
      positions.push_back(*vert->getPos());
      textCoords.push_back(*vert->getTextCoord());
  }

  glBindBuffer(GL_ARRAY_BUFFER , m_vertexArrayBuffers[POSITION_VB]);
  glBufferData(GL_ARRAY_BUFFER, getNumVertices() * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER , m_vertexArrayBuffers[TEXTCOORD_VB]);
  glBufferData(GL_ARRAY_BUFFER, getNumVertices() * sizeof(textCoords[0]), &textCoords[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  if(m_renderMask & (1 << INDEXED_OFFSET))
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDICES_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
  }

  glBindVertexArray(0);
}

void Mesh::draw()
{
  glBindVertexArray(m_vertexArrayObject);
  glDrawArrays(m_primType, 0, m_drawCount);
  glBindVertexArray(0);
}

void Mesh::drawIndexed()
{
  glBindVertexArray(m_vertexArrayObject);
  glDrawElements(m_primType, m_indices.size(), GL_UNSIGNED_INT, (void*) 0);
  glBindVertexArray(0);
}
}
