//
//  Input.cpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#include <stdio.h>
#include "../includes/Asuna/Input.hpp"
#include "../includes/Asuna/Transform.hpp"
#include "../includes/Asuna/Camera.hpp"

namespace Asuna
{
  Input::Input(InputType type) : m_type(type)
  {
    if(type == ALL_INPUT)
    {
      for(int i = 0; i < ALL_INPUT; i++)
      {
        m_inputState[i] = true;
      }
    }
    else
    {
      m_inputState[type] = true;
    }
  }

  void Input::poll()
  {
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
      if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEWHEEL)
      {
        handleMouseEvent(e);
      }
      else if(e.type == SDL_WINDOWEVENT)
      {
        handleWindowEvent(e);
      }
    }
    handleKeyboardState();
  }

  void Input::handleKeyboardState()
  {
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    if(keyboardState[SDL_SCANCODE_LEFT])
        m_transform->getPos() += 10.0f * glm::normalize(*m_right);
    if(keyboardState[SDL_SCANCODE_RIGHT])
        m_transform->getPos() -= 10.0f * glm::normalize(*m_right);
    if(keyboardState[SDL_SCANCODE_UP])
        m_transform->getPos() -= 10.0f * glm::normalize(*m_up);
    if(keyboardState[SDL_SCANCODE_DOWN])
        m_transform->getPos() += 10.0f * glm::normalize(*m_up);
    if(keyboardState[SDL_SCANCODE_X])
        m_transform->getPos() += 10.0f * glm::normalize(*m_forward);
    if(keyboardState[SDL_SCANCODE_Z])
        m_transform->getPos() -= 10.0f * glm::normalize(*m_forward);

  }

  void Input::PrintKeyInfo( SDL_KeyboardEvent *key )
  {
      if( key->type == SDL_KEYUP )
      {
          printf( "Release:- " );
      }
      else
      {
          printf( "Press:- " );
      }

      printf("Scancode: 0x%02X", key->keysym.scancode);
      printf(", Name: %s\n", SDL_GetKeyName(key->keysym.sym));
      PrintModifiers( key->keysym.mod );
  }

  void Input::PrintModifiers(long mod)
  {
      printf( "Modifers: " );

      if( mod == KMOD_NONE ){
          printf( "None\n" );
          return;
      }

      if( mod & KMOD_NUM ) printf( "NUMLOCK " );
      if( mod & KMOD_CAPS ) printf( "CAPSLOCK " );
      if( mod & KMOD_LCTRL ) printf( "LCTRL " );
      if( mod & KMOD_RCTRL ) printf( "RCTRL " );
      if( mod & KMOD_RSHIFT ) printf( "RSHIFT " );
      if( mod & KMOD_LSHIFT ) printf( "LSHIFT " );
      if( mod & KMOD_RALT ) printf( "RALT " );
      if( mod & KMOD_LALT ) printf( "LALT " );
      if( mod & KMOD_CTRL ) printf( "CTRL " );
      if( mod & KMOD_SHIFT ) printf( "SHIFT " );
      if( mod & KMOD_ALT ) printf( "ALT " );
      printf( "\n" );
  }

  void Input::handleMouseEvent(const SDL_Event& e)
  {
    printf("Handling mouse event\n");
    switch(e.type)
    {
      case SDL_MOUSEBUTTONDOWN:
        switch(e.button.button)
        {
          case SDL_BUTTON_LEFT:
            printf("Left button was pressed\n");
            break;
        case SDL_BUTTON_RIGHT:
            printf("Right button was pressed\n");
            break;
        default:
            printf("Some other button was pressed\n");
            break;
        }
        break;
      case SDL_MOUSEBUTTONUP:
          switch(e.button.button)
          {
            case SDL_BUTTON_LEFT:
              printf("Left button was released\n");
              break;
          case SDL_BUTTON_RIGHT:
              printf("Right button was released\n");
              break;
          default:
              printf("Some other button was released\n");
              break;
          }
          break;
    }
  }

  void Input::handleWindowEvent(const SDL_Event& event)
  {
    switch(event.window.event)
    {
      case SDL_WINDOWEVENT_SHOWN:
          printf("Window %d shown", event.window.windowID);
          break;
      case SDL_WINDOWEVENT_HIDDEN:
          printf("Window %d hidden", event.window.windowID);
          break;
      case SDL_WINDOWEVENT_EXPOSED:
          printf("Window %d exposed", event.window.windowID);
          break;
      case SDL_WINDOWEVENT_MOVED:
          printf("Window %d moved to %d,%d",
                  event.window.windowID, event.window.data1,
                  event.window.data2);
          break;
      case SDL_WINDOWEVENT_RESIZED:
          printf("Window %d resized to %dx%d",
                  event.window.windowID, event.window.data1,
                  event.window.data2);
          break;
      case SDL_WINDOWEVENT_SIZE_CHANGED:
          printf("Window %d size changed to %dx%d",
                  event.window.windowID, event.window.data1,
                  event.window.data2);
          break;
      case SDL_WINDOWEVENT_MINIMIZED:
          printf("Window %d minimized", event.window.windowID);
          break;
      case SDL_WINDOWEVENT_MAXIMIZED:
          printf("Window %d maximized", event.window.windowID);
          break;
      case SDL_WINDOWEVENT_RESTORED:
          printf("Window %d restored", event.window.windowID);
          break;
      case SDL_WINDOWEVENT_ENTER:
          printf("Mouse entered window %d",
                  event.window.windowID);
          break;
      case SDL_WINDOWEVENT_LEAVE:
          printf("Mouse left window %d", event.window.windowID);
          break;
      case SDL_WINDOWEVENT_FOCUS_GAINED:
          printf("Window %d gained keyboard focus",
                  event.window.windowID);
          break;
      case SDL_WINDOWEVENT_FOCUS_LOST:
          printf("Window %d lost keyboard focus",
                  event.window.windowID);
          break;
      case SDL_WINDOWEVENT_CLOSE:
          printf("Window %d closed", event.window.windowID);
          break;
      default:
          printf("Window %d got unknown event %d",
                  event.window.windowID, event.window.event);
          break;
    }
    printf("\n");
  }
}
