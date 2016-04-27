//
//  Shader.hpp
//  Asuna
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include "Transform.hpp"
#include "Camera.hpp"
#include <string>
#include <GL/glew.h>

namespace Asuna{
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
}
