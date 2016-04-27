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



namespace Asuna{
Mesh::Mesh(Vertex* vertices, unsigned int numVertices, GLenum drawType){

    m_drawCount = numVertices;
    m_drawType = drawType;
    glGenVertexArraysAPPLE(1, &m_vertexArrayObject);
    glBindVertexArrayAPPLE(m_vertexArrayObject);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textCoords;

    positions.reserve(numVertices);
    textCoords.reserve(numVertices);

    for(int i = 0; i < numVertices; i++){
        positions.push_back(*vertices[i].getPos());
        textCoords.push_back(*vertices[i].getTextCoord());
    }

    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glBindBuffer(GL_ARRAY_BUFFER , m_vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    // each attribute has 3 pieces of data, vec3, stored as floating points. false for normalizing
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER , m_vertexArrayBuffers[TEXTCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(textCoords[0]), &textCoords[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    // each attribute has 2 pieces of data, vec2, stored as floating points. false for normalizing
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArrayAPPLE(0);
}

Mesh::~Mesh(){
    glDeleteVertexArraysAPPLE(1,&m_vertexArrayObject);
}

void Mesh::draw(){
    glBindVertexArrayAPPLE(m_vertexArrayObject);

    glDrawArrays(m_drawType, 0, m_drawCount);

    //unbind
    glBindVertexArrayAPPLE(0);
}
}
