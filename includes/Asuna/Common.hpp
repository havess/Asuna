//
//  Common.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//


#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../../external/glm/glm.hpp"
#include <GL/glew.h>

namespace Asuna
{
using Float32 = float;
using Float64 = double;

template<typename T>
using sp = std::shared_ptr<T>;
template<typename T>
using up = std::unique_ptr<T>;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
template<typename T>
using vector = std::vector<T>;
template<typename T, typename N>
using map = std::map<T, N>;
template<typename T, typename N>
using pair = std::pair<T, N>;
}
