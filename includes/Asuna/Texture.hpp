//
//  Texture.hpp
//  OpenGLTut
//
//  Created by Sam Haves on 2015-12-21.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <string>
#include <GL/glew.h>

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
#endif /* Texture_hpp */
