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

namespace Asuna{


struct HUD_Data{
  HUD_Data(const vec2 dimensions, const std::string name): name(name), dimensions(dimensions){}
  std::string name;
  bool initialized = false;
  vec2 dimensions;
  double time = 0.0f;
  bool mousePressed[3] = { false, false, false };
  float mouseWheel = 0.0f;
  GLuint fontTexture = 0;
};

class HUD{
public:
  HUD(const vec2 dimensions, const std::string title = DEFAULT_TITLE);
  virtual ~HUD();

  bool init();
  void beginHUD();
  void endHUD();
  bool shutdown();
  bool processEvent(SDL_Event* event);

  //************ inline funcs ******************//
  inline HUD_Data* getData() const{return  m_hudData;}
private:
  bool createDevices();
  bool invalidateDevices();
  static HUD_Data* m_hudData;
};
}
