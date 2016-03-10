//
//  Mesh.hpp
//  OpenGLTut
//
//  Created by Sam Haves on 2015-12-18.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"

struct HalfEdge;
struct HE_Vert;
struct HE_Face;

class Vertex{
public:
    Vertex(const glm::vec3& pos, const glm::vec2& texCoord){
        m_pos = pos;
        m_textCoord = texCoord;
    }
    
    void normalize();
    
    inline glm::vec3* getPos(){
        return &m_pos;
    }
    
    inline void setPos(glm::vec3 pos){m_pos =pos;};
    
    inline glm::vec2* getTextCoord(){
        return &m_textCoord;
    }
    
protected:
private:
    glm::vec3 m_pos;
    glm::vec2 m_textCoord;
};

struct HalfEdge{
    std::shared_ptr<HalfEdge> next , previous, pair;
    std::shared_ptr<HE_Vert> origin;
    std::shared_ptr<HE_Face> face;
};

struct HE_Vert{
    glm::vec3 position;
    std::shared_ptr<HalfEdge> edge;
};

struct HE_Face{
    glm::vec3 normal;
    std::shared_ptr<HalfEdge> edge;
    long int id;
};

class Mesh{
public:
    Mesh(Vertex* vertices = nullptr, unsigned int numVertices = 0, GLenum drawType = GL_TRIANGLES);
    virtual ~Mesh();
    //void operator =(const Mesh& other){}
    //Mesh(const Mesh& other){}
    inline int getNumVertices(){return m_drawCount;};
    void draw();
protected:
private:
    static const unsigned int NUM_BUFFERS = 2;
    
    enum{
        POSITION_VB,
        TEXTCOORD_VB
    };
    
    GLuint m_vertexArrayObject;
    GLuint m_vertexArrayBuffers[NUM_BUFFERS];
    unsigned int m_drawCount;
    GLenum m_drawType;
};

#endif /* Mesh_hpp */
