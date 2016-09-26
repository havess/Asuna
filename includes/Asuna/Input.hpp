//
//  Input.hpp
//  Asuna
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include "Transform.hpp"
#include "Camera.hpp"

namespace Asuna{

enum InputType
{
  KEYBOARD,
  MOUSE,
  ALL_INPUT
};

class Input{
public:
  Input(InputType input);
  void poll();

  inline void lock(InputType type) { m_inputState[type] = false; }
  inline void unlock(InputType type) { m_inputState[type] = true; }
  inline void setInputType(InputType type) { m_type = type; }
  inline void bindTransform(Transform* transform) { m_transform = transform; }
  inline void bindForward(vec3* forward) { m_forward = forward; }
  inline void bindUp(vec3* up) { m_up = up; }
  inline void bindRight(vec3* right) { m_right = right; }
private:
  void handleWindowEvent(const SDL_Event& e);
  void handleMouseEvent(const SDL_Event& e);
  void handleKeyboardState();
  Transform*  m_transform;
  vec3*       m_forward;
  vec3*       m_up;
  vec3*       m_right;
  bool        m_inputState[ALL_INPUT] = {false};
  InputType   m_type = ALL_INPUT;

  void PrintKeyInfo(SDL_KeyboardEvent *key);
  void PrintModifiers(long mod);
};
}
