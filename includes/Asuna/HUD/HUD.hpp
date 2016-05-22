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
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "../../../external/imgui/imgui.h"

template<typename T>
using sp = std::shared_ptr<T>;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
template<typename T>
using vector = std::vector<T>;
namespace Asuna{

struct HUD_Data{
  vec2 dimensions;
  ImDrawData* draw_data = ImGui::GetDrawData();
};

class HUD{
public:
  HUD();
  virtual ~HUD();

  bool initHUD(const float width, const float height);
  void update();

  //************ inline funcs ******************//
  inline HUD_Data getData() const{ return  m_hudData;}
protected:
private:
  void RenderDrawLists(ImDrawData* draw_data);
  bool createDevices();
  HUD_Data m_hudData;
};
