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
  if(!m_mesh) { genMesh(); }
  if(m_mesh->getRenderMask() & (1 << INDEXED_OFFSET))
  {
     m_mesh->drawIndexed();
  }
  else
  {
    m_mesh->draw();
  }
}

void IcoSphereGenerator::genVertices(){

    vector<TriangleFace> faces = getIcosahedronFaces();

    for(int i = 0; i < m_iterations; i++){
        vector<TriangleFace> faces2;
        for(vector<TriangleFace>::iterator iter = faces.begin(); iter != faces.end(); ++iter){
            TriangleFace face = *iter;

            int v1 = (int) getMidpoint(face.v1, face.v2);
            int v2 = (int) getMidpoint(face.v2, face.v3);
            int v3 = (int) getMidpoint(face.v3, face.v1);

            faces2.push_back(addFace(face.v1, v1, v3));
            faces2.push_back(addFace(face.v2, v2, v1));
            faces2.push_back(addFace(face.v3, v3, v2));
            faces2.push_back(addFace(v1, v2, v3));

        }
        faces = faces2;
    }

    std::vector<Vertex> meshVerts;
    for(std::vector<TriangleFace>::iterator iter = faces.begin(); iter != faces.end(); ++iter){
        TriangleFace face = *iter;
        meshVerts.push_back(m_vertices.at(face.v1));
        meshVerts.push_back(m_vertices.at(face.v2));
        meshVerts.push_back(m_vertices.at(face.v3));
    }


    for(std::vector<Vertex>::iterator iter = meshVerts.begin(); iter != meshVerts.end(); ++iter){
        iter->getPos()->x += m_position.x;
        iter->getPos()->y += m_position.y;
        iter->getPos()->z += m_position.z;
    }
    m_vertices = meshVerts;
}


}
