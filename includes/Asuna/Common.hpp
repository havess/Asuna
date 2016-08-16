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
#include <map>
#include "glm/glm.hpp"
#include <GL/glew.h>

void CheckOpenGLError(const char* fname, int line);

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
using mat4 = glm::mat4;
using mat3 = glm::mat3; 
template<typename T>
using vector = std::vector<T>;
template<typename T, typename N>
using map = std::map<T, N>;
template<typename T, typename N>
using pair = std::pair<T, N>;
}
