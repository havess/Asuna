#include <stdio.h>
#include <iostream>

#include "../../includes/Asuna/Geometry/Sphere.hpp"

namespace Asuna
{
  void Sphere::genMesh()
  {
    m_mesh = std::make_shared<Mesh>(Mesh(GL_TRIANGLES, DRAW_INDEXED));
    double t = (1.0 + sqrt(5.0)) / 2.0;

    Vertex* vertices[] = {
        new Vertex(vec3(-1,  t,  0), vec2(0,0)),
        new Vertex(vec3( 1,  t,  0), vec2(0,0)),
        new Vertex(vec3(-1, -t,  0), vec2(0,0)),
        new Vertex(vec3( 1, -t,  0), vec2(0,0)),

        new Vertex(vec3( 0, -1,  t), vec2(0,0)),
        new Vertex(vec3( 0,  1,  t), vec2(0,0)),
        new Vertex(vec3( 0, -1, -t), vec2(0,0)),
        new Vertex(vec3( 0,  1, -t), vec2(0,0)),

        new Vertex(vec3( t,  0, -1), vec2(0,0)),
        new Vertex(vec3( t,  0,  1), vec2(0,0)),
        new Vertex(vec3(-t,  0, -1), vec2(0,0)),
        new Vertex(vec3(-t,  0,  1), vec2(0,0))
    };

    for(int i = 0 ; i < 12; i++){
        normalize(vertices[i], m_radius);
        m_mesh->addVertex(vertices[i]);
    }

    m_mesh->addTriangle(0, 11, 5);
    m_mesh->addTriangle(0, 5, 1);
    m_mesh->addTriangle(0, 1, 7);
    m_mesh->addTriangle(0, 7, 10);
    m_mesh->addTriangle(0, 10, 11);
    // 5 adjacent faces
    m_mesh->addTriangle(1, 5, 9);
    m_mesh->addTriangle(5, 11, 4);
    m_mesh->addTriangle(11, 10, 2);
    m_mesh->addTriangle(10, 7, 6);
    m_mesh->addTriangle(7, 1, 8);
    // 5 faces around point 3
    m_mesh->addTriangle(3, 9, 4);
    m_mesh->addTriangle(3, 4, 2);
    m_mesh->addTriangle(3, 2, 6);
    m_mesh->addTriangle(3, 6, 8);
    m_mesh->addTriangle(3, 8, 9);
    // 5 adjacent faces
    m_mesh->addTriangle(4, 9, 5);
    m_mesh->addTriangle(2, 4, 11);
    m_mesh->addTriangle(6, 2, 10);
    m_mesh->addTriangle(8, 6, 7);
    m_mesh->addTriangle(9, 8, 1);
    int  n = 0;
    for(int i = 0; i < SPHERE_ITERATIONS; i++)
    {
      n = m_mesh->getNumIndices();
      for(int j = 0; j < n; j += 3)
      {
        /*printf("getting midpoint i = %d : %d i+1: %d i+2: %d numVertices: %d numIndices: %d\n", i, m_mesh->getIndex(j), m_mesh->getIndex(j+1), m_mesh->getIndex(j+2),
          m_mesh->getNumVertices(), m_mesh->getNumIndices());*/
        unsigned int v1 = getMidpoint(m_mesh->getIndex(j), m_mesh->getIndex(j+1)),
                     v2 = getMidpoint(m_mesh->getIndex(j), m_mesh->getIndex(j+2)),
                     v3 = getMidpoint(m_mesh->getIndex(j+1), m_mesh->getIndex(j+2));
        m_mesh->addTriangle(v1, v2, v3);
        m_mesh->addTriangle(v1, v2, m_mesh->getIndex(j));
        m_mesh->addTriangle(v1, v3, m_mesh->getIndex(j+1));
        m_mesh->addTriangle(v2, v3, m_mesh->getIndex(j+2));
      }
      m_mesh->eraseIndices(0, n);
    }
    m_mesh->mapBuffers();
    printf("Sphere has %d vertices and %d indices\n", m_mesh->getNumVertices(), m_mesh->getNumIndices());
  }

  void Sphere::normalize(Vertex* vert, const float length)
  {
    vec3* pos = vert->getPos();
    double len = sqrt(pos->x * pos->x + pos->y * pos->y + pos->z * pos->z);
    vert->setPos(vec3(length * (pos->x/len), length * (pos->y/len), length * (pos->z/len)));
  }
  unsigned int Sphere::getMidpoint(const unsigned int index1, const unsigned int index2)
  {
    //get the smaller index since smaller is computed firsts
    bool smaller = index1 < index2;
    long int smallIndex = smaller ? index1:index2;
    long int bigIndex = smaller ? index2:index1;

    //midpoints are stored with long int with both end point indices
    long int key = (smallIndex << 32) + bigIndex;

    if(m_middlePointCache.find(key) != m_middlePointCache.end()){
        return m_middlePointCache.at(key);
    }

    vec3 posA = *m_mesh->getVertex(index1)->getPos(), posB = *m_mesh->getVertex(index2)->getPos();
    vec3 mid = vec3((posA.x + posB.x)/2, (posA.y + posB.y)/2, (posA.z + posB.z)/2);

    Vertex* output = new Vertex(mid, vec2(0,0));
    normalize(output, m_radius);
    m_mesh->addVertex(output);

    m_middlePointCache.insert(pair<long int, int>(key, m_mesh->getNumVertices() - 1));

    return m_mesh->getNumVertices() - 1;
  }
}
