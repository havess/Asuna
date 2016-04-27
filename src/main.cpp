//
//  main.cpp
//  Asuna
//  Created By Sam Haves
//


#include <iostream>
#include <vector>
#include <random>
#include <GL/glew.h>

#include "../includes/Asuna/Asuna.hpp"

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;


int main(int argc, const char * argv[]) {
    Asuna::Display display(vec2(1000,700), "3D QuickHull");
    Asuna::Camera camera(vec3(-200,-300,200), 70.0f, display.getAspectRatio(), 0.0f, 2000.0f);
    SDL_Quit();
    return 0;
}
