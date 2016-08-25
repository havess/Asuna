//
//  ProgramObject.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include "Common.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include <string>
#include <GL/glew.h>

namespace Asuna{

enum ShaderType {
  ASUNA_VERTEX_SHADER,
  ASUNA_FRAGMENT_SHADER,
  ASUNA_NUM_SUPPORTED_SHADERS
};

class ProgramObject{
public:
    ProgramObject();
    virtual ~ProgramObject();

    void link();
    void bind();
    void createVertexShader(const string& absolutePath);
    void createFragmentShader(const string& absolutePath);
    void deleteVertexShader();
    void deleteFragmentShader();
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
protected:
private:

    GLuint  m_program;
    //Shader table, single pass for the moment, might want to make it array of lists.
    GLuint  m_shaders[ASUNA_NUM_SUPPORTED_SHADERS];

    string loadShaderFromFile(const string& absolutePath);
    GLuint createShader(const std::string& text, GLenum shaderType);
    void   checkProgramError(GLuint shader, GLuint flag, const std::string& errMessage);
    void   checkShaderError(GLuint shader, GLuint flag, const std::string& errMessage);
};
}
