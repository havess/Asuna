//
//  HUD.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

/*
This is the general hud class. This will be what is instantied for the HUD overlay and where everything is handled.
*/

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include "../Common.hpp"
#include "../ProgramObject.hpp"
#include "../../../external/imgui/imgui.h"

template<typename T>
using sp = std::shared_ptr<T>;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
template<typename T>
using vector = std::vector<T>;

#ifndef HUD_DEFS
#define HUD_DEFS

#define DEFAULT_TITLE "Asuna HUD"

#endif

namespace Asuna {

class HUD
{
public:
  HUD(SDL_Window* window, const std::string title = DEFAULT_TITLE);
  virtual ~HUD();

  bool init();
  void beginHUD();
  void endHUD();
  bool shutdown();
  bool processEvent(SDL_Event* event);

private:
  bool createFontsTexture();
  bool createDevices();
  bool invalidateDevices();

  ProgramObject m_pObj;
};
}
