//
//  Shader.cpp
//  OpenGLTut
//
//  Created by Sam Haves on 2015-12-18.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#include "Shader.hpp"
#include <fstream>
#include <iostream>

static std::string loadShader(const std::string& filename);
static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errMessage);
static GLuint createShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& filename){
    m_program = glCreateProgram();
    m_shaders[0] = createShader(loadShader(filename + ".vs"), GL_VERTEX_SHADER);
    m_shaders[1] = createShader(loadShader(filename +".fs"), GL_FRAGMENT_SHADER);
    
    for(unsigned int i = 0; i < NUM_SHADERS; i++ ){
        glAttachShader(m_program, m_shaders[i]);
    }
    
    glBindAttribLocation(m_program, 0, "position");
    glBindAttribLocation(m_program, 1, "texCoord");
    
    glLinkProgram(m_program);
    checkShaderError(m_program, GL_LINK_STATUS, true, "Error: Program failed to link!");
    
    glValidateProgram(m_program);
    checkShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid!");
    
    m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
}

Shader::~Shader(){
    for(int i = 0; i < NUM_SHADERS; i++)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_program);
}

static std::string loadShader(const std::string& filename){
    
    std::ifstream file;
    file.open((filename).c_str());
    
    std::string output;
    std::string line;
    
    if(file.is_open()){
        while(file.good()){
            getline(file, line);
            output.append(line + "\n");
        }
    }else{
        std::cerr << "Shader could not be loaded, shader id: " << filename <<  std::endl;
    }
    file.close();
    return output;
}

static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errMessage){
    GLint success = 0;
    GLchar error[1024] = {0};
    
    if(isProgram){
        glGetProgramiv(shader, flag, &success);
    }else{
        glGetShaderiv(shader, flag, &success);
    }
    
    if(success == GL_FALSE){
        if(isProgram){
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        }else{
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        }
        
        std::cerr << errMessage << ": " << error << "'" << std::endl;
    }
}

static GLuint createShader(const std::string& text, GLenum shaderType){
    GLuint shader = glCreateShader(shaderType);
    
    if (shader == 0) std::cerr << "The program ran into an error creating the shader" << std::endl;
    
    const GLchar* shaderSourceStrings[1];
    GLint shaderSourceStringLens[1];
    
    shaderSourceStrings[0] = text.c_str();
    shaderSourceStringLens[0] = (int) text.length();
    
    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLens);
    glCompileShader(shader);
    
    checkShaderError(shader, GL_COMPILE_STATUS, false, "Error: compilation failed!");
    return shader;
}

void Shader::bind(){
    glUseProgram(m_program);
}

void Shader::update(const Transform& transform, const Camera& camera){
    glm::mat4 model = camera.getViewProjection() * transform.getModel();
    
    
    glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}