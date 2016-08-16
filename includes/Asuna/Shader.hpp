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
    Shader(ShaderType type);
    virtual ~Shader();

    bool loadFromFile(const char* fileName, GLenum shaderType);
    bool loadFromString(const std::string& shaderString, GLenum shaderType);
    bool link();
    void bind();
    void bindAttribLocation( GLuint location, const char* name);
    void bindFragDataLocation( GLuint location, const char* name );
    void setUniform(const char* name,float x,float y, float z);
    void setUniform(const char* name, const vec3 & v);
    void setUniform(const char* name, const vec4 & v);
    void setUniform(const char* name, const mat4 & m);
    void setUniform(const char* name, const mat3 & m);
    void setUniform(const char* name, float val );
    void setUniform(const char* name, int val );
    void setUniform(const char* name, bool val );
    void printActiveUniforms();
    void printActiveAttribs();
    void update(const Transform& transform, const Camera& camera);

    inline GLuint getHandle() { return m_program; }
protected:
private:

    GLuint  m_program;
    GLenum  m_type;

    int handle;
    bool linked;
    string logString;
    int getUniformLocation(const char * name );
    bool fileExists( const string & fileName );
    void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errMessage);
    GLuint createShader(const std::string& text, GLenum shaderType);
};
}
