//
//  Geometry.cpp
//  Convex Hull
//
//  Created by Sam Haves on 2015-12-27.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#include <stdio.h>
#include <iostream>

#include "../includes/Asuna/Geometry.hpp"

void IcoSphereGenerator::genVertices(){

    std::vector<TriangleFace> faces = getIcosahedronFaces();

    for(unsigned int i = 0; i < m_iterations; i++){
        std::vector<TriangleFace> faces2;
        for(std::vector<TriangleFace>::iterator iter = faces.begin(); iter != faces.end(); ++iter){
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

TriangleFace IcoSphereGenerator::addFace(const unsigned int index1, const unsigned int index2, const unsigned int index3){
    return TriangleFace(index1, index2, index3);
}


void IcoSphereGenerator::normalize(Vertex& vertex, const float length){
    glm::vec3 pos = *vertex.getPos();
    double len = sqrt(pos.x*pos.x + pos.y*pos.y + pos.z*pos.z);
    *vertex.getPos() = glm::vec3(length*(pos.x/len), length*(pos.y/len), length*(pos.z/len));
}

long int IcoSphereGenerator::getMidpoint(int iA, int iB){

    //get the smaller index since smaller is computed first
    bool smaller = iA < iB;
    long int smallIndex = smaller ? iA:iB;
    long int bigIndex = smaller ? iB:iA;

    //midpoints are stored with long int with both end point indices
    long int key = (smallIndex << 32) + bigIndex;

    if(m_middlePointCache.find(key) != m_middlePointCache.end()){
        return m_middlePointCache.at(key);
    }

    glm::vec3 posA = *m_vertices[iA].getPos();
    glm::vec3 posB = *m_vertices[iB].getPos();
    glm::vec3 mid = glm::vec3((posA.x + posB.x)/2, (posA.y + posB.y)/2, (posA.z + posB.z)/2);

    Vertex output(mid, glm::vec2(0,0));
    normalize(output, m_radius);
    m_vertices.push_back(output);

    m_middlePointCache.insert(std::pair<long int, int>(key, m_vertices.size() - 1));

    return m_vertices.size() - 1;
}

std::vector<TriangleFace> IcoSphereGenerator::getIcosahedronFaces(){

    double t = (1.0 + sqrt(5.0)) / 2.0;

    Vertex vertices[] = {
        Vertex(glm::vec3(-1,  t,  0), glm::vec2(0,0)),
        Vertex(glm::vec3( 1,  t,  0), glm::vec2(0,0)),
        Vertex(glm::vec3(-1, -t,  0), glm::vec2(0,0)),
        Vertex(glm::vec3( 1, -t,  0), glm::vec2(0,0)),

        Vertex(glm::vec3( 0, -1,  t), glm::vec2(0,0)),
        Vertex(glm::vec3( 0,  1,  t), glm::vec2(0,0)),
        Vertex(glm::vec3( 0, -1, -t), glm::vec2(0,0)),
        Vertex(glm::vec3( 0,  1, -t), glm::vec2(0,0)),

        Vertex(glm::vec3( t,  0, -1), glm::vec2(0,0)),
        Vertex(glm::vec3( t,  0,  1), glm::vec2(0,0)),
        Vertex(glm::vec3(-t,  0, -1), glm::vec2(0,0)),
        Vertex(glm::vec3(-t,  0,  1), glm::vec2(0,0))
    };

    for(unsigned int i = 0 ; i < 12; i++){
        normalize(vertices[i], m_radius);
        m_vertices.push_back(vertices[i]);
    }

    std::vector<TriangleFace> faces;
    faces.reserve(20);

    faces.push_back(addFace(0, 11, 5));
    faces.push_back(addFace(0, 5, 1));
    faces.push_back(addFace(0, 1, 7));
    faces.push_back(addFace(0, 7, 10));
    faces.push_back(addFace(0, 10, 11));

    // 5 adjacent faces
    faces.push_back(addFace(1, 5, 9));
    faces.push_back(addFace(5, 11, 4));
    faces.push_back(addFace(11, 10, 2));
    faces.push_back(addFace(10, 7, 6));
    faces.push_back(addFace(7, 1, 8));

    // 5 faces around point 3
    faces.push_back(addFace(3, 9, 4));
    faces.push_back(addFace(3, 4, 2));
    faces.push_back(addFace(3, 2, 6));
    faces.push_back(addFace(3, 6, 8));
    faces.push_back(addFace(3, 8, 9));

    // 5 adjacent faces
    faces.push_back(addFace(4, 9, 5));
    faces.push_back(addFace(2, 4, 11));
    faces.push_back(addFace(6, 2, 10));
    faces.push_back(addFace(8, 6, 7));
    faces.push_back(addFace(9, 8, 1));

    for(std::vector<TriangleFace>::iterator iter = faces.begin(); iter != faces.end(); ++iter){
        TriangleFace face = *iter;
        m_vertices.push_back(m_vertices.at(face.v1));
        m_vertices.push_back(m_vertices.at(face.v2));
        m_vertices.push_back(m_vertices.at(face.v3));
    }
    return faces;
}

Sphere::Sphere(glm::vec3 position, const float radius): Geometry(position){
    m_radius = radius;
    m_icoGen = IcoSphereGenerator(position, radius, 2);
    m_icoGen.genVertices();
}

void Sphere::genMesh(){
    m_mesh = std::shared_ptr<Mesh>(new Mesh(&m_icoGen.getVertices()[0], (int) m_icoGen.getNumVertices(), GL_TRIANGLES));
}
