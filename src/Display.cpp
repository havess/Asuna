//
//  Display.cpp
//  OpenGLTut
//
//  Created by Sam Haves on 2015-12-17.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "../includes/Asuna/Display.hpp"

namespace Asuna{

Display::Display(const glm::vec2& size, const std::string& title, const bool hud)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    //at least 8 bits per, BUFFER_SIZE = 8+8+8+8, for pixel data
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    //allocate space for a second window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        size.x, size.y, SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);
    if(hud){
      m_hud = new HUD(m_window, "Asuna");
      m_hud->init();
    }

    //this is doing weird stuff on mac, figure it out later
  //  SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    //SDL_ShowCursor(true);
    glewExperimental = GL_TRUE;
    GLenum status = glewInit();

    if(status != GLEW_OK){
        std::cerr << "Glew could not be initialize" <<std::endl;
    }

    m_isClosed = false;
    m_aspectRatio = (float) size.x/size.y;
}

Display::~Display()
{
    delete m_hud;
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Display::isClosed() const
{
    m_hud->beginHUD();
    return m_isClosed;
}

void Display::clear(float r, float g, float b, float a)
{
   glClearColor(r, g, b, a);
   glClear(GL_COLOR_BUFFER_BIT);
}

void Display::update()
{
    //m_hud->endHUD();
    SDL_GL_SwapWindow(m_window);

    SDL_Event e;

    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT){
          //m_hud->processEvent(&e);
            m_isClosed = true;
        }
    }
}
}
