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

template<typename T>
using sp = std::shared_ptr<T>;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
using vector = std::std::vector<T>;

namespace Asuna{
class HUD{
public:
  HUD();
  virtual ~HUD();
  initHUD();
  update();
protected:
private:

};
}
