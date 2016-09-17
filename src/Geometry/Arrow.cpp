//
//  Arrow.cpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include "../../includes/Asuna/Asuna.hpp"

namespace Asuna{
  void Arrow::genMesh()
  {
    Vertex vertices[] =
    {
        Vertex(m_position, vec2(0, 0)),
        Vertex(m_position + (m_direction*m_length), vec2(0, 0)),
        //blah okay so rotating vectors on a plane suck so uh yeah ill come back to this once i switch everything to quaternions.
        // Reason being im going to make the arrow head by doing some rotation but idealy the arrow head is flat on the plane
        Vertex(vec3(1,1,0), vec2(0, 0)),
        Vertex(vec3(1,-1,0), vec2(0, 0))
    };

    m_mesh = std::make_shared<Mesh>(GL_LINES, DRAW_BASIC);
  }
}
