//
//  Plane.hpp
//  Convex Hull
//
//  Created by Sam Haves on 2015-12-24.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include "glm/glm.hpp"
#include "Mesh.hpp"
#include <GL/glew.h>

class Geometry{
public:
    Geometry(const glm::vec3& position = glm::vec3(0,0,0)): m_position(position) {}
    virtual void genMesh() = 0;
    inline glm::vec3* getPosition(){return &m_position;};
    inline std::shared_ptr<Mesh> getMesh(){
        return m_mesh;};
protected:
    glm::vec3 m_position;
    //TODO: fix this ugliness
    std::shared_ptr<Mesh> m_mesh;
private:
};

class Plane: public Geometry{
public:
    Plane(const glm::vec3& position = glm::vec3(0,0,0),const glm::vec3& normal = glm::vec3(0,1,0), const float width = 400, const float height = 400): Geometry(position){
        m_normal = normal;
        m_height = height;
        m_width = width;
    }
    
    virtual void genMesh(){
        Vertex vertices[] = {
            Vertex(glm::vec3(-1,-1,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(-1,1,0), glm::vec2( 0, 1)),
            Vertex(glm::vec3(1,1,0), glm::vec2(1, 1)),
            Vertex(glm::vec3(1,-1,0), glm::vec2(1, 0))
        };
        m_mesh = std::shared_ptr<Mesh>(new Mesh(vertices, sizeof(vertices)/sizeof(Vertex), GL_QUADS));
    }
    
    virtual inline glm::vec3* getNormal(){return &m_normal;};
    virtual inline float* getHeight(){return &m_height;};
    virtual inline float* getWidth(){return &m_width;};
    
    virtual inline void setNormal(const glm::vec3& normal){ m_normal = normal;};
    virtual inline void setWidth(const float width){ m_width = width;};
    virtual inline void setHeight(const float height){ m_height = height;};
    
protected:
    glm::vec3 m_normal;
    float m_width, m_height;
private:
};

class Grid: public Plane{
public:
    Grid(const glm::vec3& position = glm::vec3(0,0,0),const glm::vec3& normal = glm::vec3(0,1,0), const float width = 1000, const float height = 1000, const bool checkered = false, const float lineSpacing = 10): Plane(position,normal,width,height){
        m_checkered = checkered;
        m_spacing = lineSpacing;
    }
    
    void genMesh(){
        std::vector<Vertex> vertices;
        
        for(int i = -m_height/2; i != m_height/2 + m_spacing; i += m_spacing){
            vertices.push_back(Vertex(glm::vec3(-m_width/2, i, 0), glm::vec2(0,0)));
            vertices.push_back(Vertex(glm::vec3(m_width/2, i, 0), glm::vec2(0,0)));
            vertices.push_back(Vertex(glm::vec3(i, -m_width/2, 0), glm::vec2(0,0)));
            vertices.push_back(Vertex(glm::vec3(i, m_width/2, 0), glm::vec2(0,0)));
        }
        
        Vertex* verts = &vertices[0];
        m_mesh = std::shared_ptr<Mesh>(new Mesh(verts, (int) vertices.size(), GL_LINES));
    }
    
protected:
private:
    bool m_checkered;
    float m_spacing;
};

class Octahedron: public Geometry{
public:
    Octahedron(glm::vec3 position = glm::vec3(0,0,0), const float height = 20.0f) : Geometry(position){
        m_height = height;
        m_len = m_height * cos(0.79);
        m_vertices = getVertices();
    }
    
    std::vector<Vertex> getVertices(){
        std::vector<Vertex> vertices = {
            Vertex(glm::vec3(-m_len,-m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(-m_len,m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(0,0,m_height), glm::vec2(0, 0)),
            Vertex(glm::vec3(-m_len,-m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(-m_len,m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(0,0,-m_height), glm::vec2(0, 0)),
            Vertex(glm::vec3(m_len,m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(m_len,-m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(0,0,m_height), glm::vec2(0, 0)),
            Vertex(glm::vec3(m_len,m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(m_len,-m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(0,0,-m_height), glm::vec2(0, 0)),
            Vertex(glm::vec3(-m_len,-m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(m_len,-m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(0,0,m_height), glm::vec2(0, 0)),
            Vertex(glm::vec3(-m_len,-m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(m_len,-m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(0,0,-m_height), glm::vec2(0, 0)),
            Vertex(glm::vec3(m_len,m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(-m_len,m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(0,0,m_height), glm::vec2(0, 0)),
            Vertex(glm::vec3(m_len,m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(-m_len,m_len,0), glm::vec2(0, 0)),
            Vertex(glm::vec3(0,0,-m_height), glm::vec2(0, 0)),
        };
        return vertices;
    }
    
    void genMesh(){
        std::cout << m_vertices.size() << std::endl;
        m_mesh = std::shared_ptr<Mesh>(new Mesh(&m_vertices[0], (int) m_vertices.size() , GL_TRIANGLES));
    }
    
    float* getHeight(){return &m_height;};
    
    void setHeight(const float height){ m_height = height;};
protected:
    
private:
    unsigned int m_numvertices;
    float m_height, m_len;
    std::vector<Vertex> m_vertices;

};

struct TriangleFace{
public:
    unsigned int v1, v2, v3;
    
    TriangleFace(unsigned int v1, unsigned int v2, unsigned int v3): v1(v1),  v2(v2), v3(v3) {}
};

class IcoSphereGenerator{
public:
    IcoSphereGenerator(const glm::vec3& position = glm::vec3(0,0,0), const float radius = 10.0f, const int iterations = 10): m_position(position), m_radius(radius), m_iterations(iterations) {}
    
    void genVertices();
    
    std::vector<Vertex> getVertices(){return m_vertices;};
    long int getNumVertices(){return m_vertices.size();};
    std::vector<TriangleFace> getIcosahedronFaces();
    
private:
    std::map<long int, int> m_middlePointCache;
    float m_radius;
    int m_iterations;
    std::vector<Vertex> m_vertices;
    glm::vec3 m_position;
    
    TriangleFace addFace(const unsigned int index1, const unsigned int index2, const unsigned int index3);
    
    
    void normalize(Vertex& vertex, const float length = 1);
    
    long int getMidpoint(int iA, int iB);
    
    
    
};

class Sphere: public Geometry{
public:
    Sphere(glm::vec3 position, const float radius);
    
    void genMesh();
    
   inline float* getRadius(){return &m_radius;};
   inline void setRadius(const float radius){ m_radius = radius;};
    
protected:
private:
    float m_radius;
    IcoSphereGenerator m_icoGen;
};


#endif /* Plane_hpp */
