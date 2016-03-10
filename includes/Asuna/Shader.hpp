//
//  Shader.hpp
//  OpenGLTut
//
//  Created by Sam Haves on 2015-12-18.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include "Transform.hpp"
#include "Camera.hpp"
#include <string>
#include <GL/glew.h>


class Shader{
public:
    Shader(const std::string& filename);
    virtual ~Shader();
    void bind();
    void update(const Transform& transform, const Camera& camera);
protected:
private:
    static const unsigned int NUM_SHADERS = 2;
    Shader(const Shader& other){}
    void operator=(const Shader& other){}
    
    enum{
        TRANSFORM_U,
        NUM_UNIFORMS
    };
    
    //program handle
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];
    
};
#endif /* Shader_hpp */
