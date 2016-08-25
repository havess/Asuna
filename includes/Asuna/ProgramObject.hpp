//
//  ProgramObject.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include "Transform.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include <string>
#include <GL/glew.h>

namespace Asuna{

enum ShaderType {
  ASUNA_VERTEX_SHADER,
  ASUNA_FRAGMENT_SHADER,
  ASUNA_NUM_SUPPORTED_SHADERS
}

class ProgramObject{
public:
    ProgramObject();
    virtual ~ProgramObject();

    void link();
    void bind();
    bool createVertexShader(const string& absolutePath);
    bool createFragmentShader(const string& absolutePath);
    bool deleteVertexShader(ShaderType type);
    bool deleteFragmentShader(ShaderType type);
    // TODO: implement these
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
    // end todo
    void update(const Transform& transform, const Camera& camera);

    inline GLuint getHandle() { return m_program; }
    /* single shader per table slot, if shader table becomes table of lists, this needs to change */
    inline Shader* getShader(ShaderType type) { return m_shaders[type]; }
protected:
private:

    GLuint  m_program;
    //Shader table, single pass for the moment, might want to make it array of lists.
    GLuint  m_shaders[ASUNA_NUM_SHADERS];

    string loadShaderFromFile(const string& absolutePath);
    GLuint createShader(const std::string& text, Glenum shaderType);
    void   checkProgramError(GLuint shader, GLuint flag, const std::string& errMessage);
    void   checkShaderError(GLuint shader, GLuint flag, const std::string& errMessage);
};
}
