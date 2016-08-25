//
//  ProgramObject.cpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#include "../includes/Asuna/ProgramObject.hpp"
#include <fstream>
#include <iostream>

namespace Asuna
{
  ProgramObject::ProgramObject()
  {
    m_program = glCreateProgram();
  }

  ProgramObject::~ProgramObject()
  {
    glDeleteProgram(m_program);
  }

  void ProgramObject::link()
  {
    glLinkProgram(m_program);
    checkProgramError(m_program, GL_LINK_STATUS, "Error: Program failed to link!");
    glValidateProgram(m_program);
    checkProgramError(m_program, GL_VALIDATE_STATUS, "Error: Program is invalid!");
    // here since the program was linked and validated we can get rid of the shader source buffers
    deleteVertexShader();
    deleteFragmentShader();
  }

  void ProgramObject::bind()
  {
    glUseProgram(m_program);
  }

  GLuint ProgramObject::createShader(const std::string &text, GLenum shaderType)
  {
    GLuint shader = glCreateShader(shaderType);

    if (shader == 0)
    {
      std::cerr << "The program ran into an error creating the shader" << std::endl;
    }

    const GLchar* shaderSourceStrings[1];
    GLint shaderSourceStringLens[1];

    shaderSourceStrings[0] = text.c_str();
    shaderSourceStringLens[0] = (int) text.length();

    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLens);
    glCompileShader(shader);

    checkShaderError(shader, GL_COMPILE_STATUS, "Error: shader compilation failed!");
    return shader;
  }

  void ProgramObject::bindAttribLocation(GLuint location, const char *name)
  {
    glBindAttribLocation(m_program, location, name);
  }

  void ProgramObject::createVertexShader(const string& absolutePath)
  {
    m_shaders[ASUNA_VERTEX_SHADER] = createShader(loadShaderFromFile(absolutePath), GL_VERTEX_SHADER);
    glAttachShader(m_program, m_shaders[ASUNA_VERTEX_SHADER]);
  }

  void ProgramObject::createFragmentShader(const string& absolutePath)
  {
    m_shaders[ASUNA_FRAGMENT_SHADER] = createShader(loadShaderFromFile(absolutePath), GL_FRAGMENT_SHADER);
    glAttachShader(m_program, m_shaders[ASUNA_FRAGMENT_SHADER]);
  }

  void ProgramObject::deleteVertexShader()
  {
    glDetachShader(m_program, m_shaders[ASUNA_VERTEX_SHADER]);
    glDeleteShader(m_shaders[ASUNA_VERTEX_SHADER]);
  }

  void ProgramObject::deleteFragmentShader()
  {
    glDetachShader(m_program, m_shaders[ASUNA_FRAGMENT_SHADER]);
    glDeleteShader(m_shaders[ASUNA_FRAGMENT_SHADER]);
  }

  void ProgramObject::setUniform(const char* name, const mat4 & m)
  {
    glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, GL_FALSE, &m[0][0]);
  }

  void ProgramObject::update(const Transform& transform, const Camera& camera)
  {
    mat4 model = camera.getViewProjection() * transform.getModel();
    setUniform("transform", model);
  }

  string ProgramObject::loadShaderFromFile(const string& absolutePath)
  {
    std::ifstream file;
    file.open((absolutePath).c_str());

    string output;
    string line;

    if(file.is_open()){
        while(file.good()){
            getline(file, line);
            output.append(line + "\n");
        }
    }else{
        std::cerr << "Shader could not be loaded, shader id: " << absolutePath <<  std::endl;
    }
    file.close();
    return output;
  }

  void ProgramObject::checkProgramError(GLuint program, GLuint flag, const std::string& errMessage)
  {
    GLint success = 0;
    GLchar error[1024] = {0};

    glGetProgramiv(program, flag, &success);

    if(success == GL_FALSE)
    {
      glGetProgramInfoLog(program, sizeof(error), NULL, error);
      std::cerr << errMessage << ": " << error << "'" << std::endl;
    }
  }

  void ProgramObject::checkShaderError(GLuint shader, GLuint flag, const std::string& errMessage)
  {
    GLint success = 0;
    GLchar error[1024] = {0};

    glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
      glGetShaderInfoLog(shader, sizeof(error), NULL, error);
      std::cerr << errMessage << ": " << error << "'" << std::endl;
    }
  }

}//namespace Asuna
