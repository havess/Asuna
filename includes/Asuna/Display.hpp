//
//  Display.hpp
//  OpenGLTut
//
//  Created by Sam Haves on 2015-12-17.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#ifndef Display_hpp
#define Display_hpp

#include <iostream>
#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include <string>

#if !defined(DISPLAY_CONSTANTS)
#define DISPLAY_CONSTANTS

    const glm::vec2 STANDARD_SIZE = glm::vec2(800,600);

#endif

class Display{
public:
    
    
    Display(const glm::vec2& size, const std::string& title);
    virtual ~Display();
    
    void update();
    void clear(float r, float g, float b, float a);
    bool isClosed() const;
    
    inline glm::vec2* getSize(){return &m_size;}
    inline float* getAspectRatio(){return &m_aspectRatio;}
    
    
protected:
private:
    
    Display(const Display& other){}
    void operator=(const Display& other){}
    
    SDL_Window* m_window;
    glm::vec2 m_size;
    SDL_GLContext m_glContext;
    float m_aspectRatio;
    bool m_isClosed;
    
};

#endif /* Display_hpp */
