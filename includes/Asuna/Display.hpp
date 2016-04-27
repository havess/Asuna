//
//  Display.hpp
//  Asuna
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include <string>

#include "Transform.hpp"
#include "Camera.hpp"

#if !defined(DISPLAY_CONSTANTS)
#define DISPLAY_CONSTANTS

    const glm::vec2 STANDARD_SIZE = glm::vec2(800,600);

#endif

namespace Asuna{

class Display{
public:

    Display(const glm::vec2& size, const std::string& title);
    virtual ~Display();

    void update();
    void clear(float r, float g, float b, float a);
    bool isClosed() const;

    inline void bindWorldMatrix(Transform& worldMat){m_worldMat = worldMat;}
    inline void bindCamera(Camera& camera){m_camera = camera;}
    inline glm::vec2 getSize() const{return m_size;}
    inline float getAspectRatio() const{return m_aspectRatio;}


protected:
private:

    Display(const Display& other){}
    void operator=(const Display& other){}

    void handleInput();

    SDL_Window* m_window;
    glm::vec2 m_size;
    SDL_GLContext m_glContext;
    float m_aspectRatio;
    bool m_isClosed;

    Transform m_worldMat;
    Camera m_camera;

};
}
