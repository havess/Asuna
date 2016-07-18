#include <stdio.h>
#include <iostream>

#include "../../includes/Asuna/Asuna.hpp"

namespace Asuna{
  Plane::genMesh()
  {
    Vertex vertices[] = {
        Vertex(vec3(-1,-1,0), vec2(0, 0)),
        Vertex(vec3(-1,1,0), vec2( 0, 1)),
        Vertex(vec3(1,1,0), vec2(1, 1)),
        Vertex(vec3(1,-1,0), vec2(1, 0))
    };
    m_mesh = new Mesh(vertices, sizeof(vertices)/sizeof(Vertex), GL_QUADS));
  }



}
