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

void Geometry::render()
{
  //generate mesh and set initial world position
  if(!m_mesh)
  {
    m_mesh->setOffset(m_position);
    genMesh();
    m_mesh->mapBuffers();
  }

  if(m_mesh->getRenderMask() & (1 << INDEXED_OFFSET))
  {
     m_mesh->drawIndexed();
  }
  else
  {
    m_mesh->draw();
  }
}
}
