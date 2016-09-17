//
//  Geometry.cpp
//  Convex Hull
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include "../../includes/Asuna/Asuna.hpp"

namespace Asuna{

void Geometry::::normalize(Vertex* vert, float length)
{
  vec3* pos = vert->getPos();
  double len = sqrt(pos->x * pos->x + pos->y * pos->y + pos->z * pos->z);
  vert->setPos(vec3(length * (pos->x/len), length * (pos->y/len), length * (pos->z/len)));
}

void Geometry::render()
{
  //generate mesh and set initial world position
  if(!m_mesh)
  {
    genMesh();
    m_mesh->setOffset(m_position);
    m_mesh->mapBuffers();
  }

  if(m_mesh->getRenderMask() == DRAW_INDEXED)
  {
     m_mesh->drawIndexed();
  }
  else
  {
    m_mesh->draw();
  }
}
}
