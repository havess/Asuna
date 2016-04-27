//
//  Texture.hpp
//  Asuna
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
namespace Asuna{
class Texture{
public:

    Texture(const std::string& filename);
    virtual ~Texture();
    void bind();

protected:
private:
    Texture(const Texture& other){}
    void operator=(const Texture& other){}

    GLuint m_texture;
};
}
