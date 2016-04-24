//
//  main.cpp
//  Asuna
//  Created By Sam Haves
//


#include <iostream>
#include <vector>
#include <random>

#include <GL/glew.h>

#include "../includes/Asuna/Display.hpp"
#include "../includes/Asuna/Shader.hpp"
#include "../includes/Asuna/Texture.hpp"
#include "../includes/Asuna/Mesh.hpp"
#include "../includes/Asuna/QuickHull.hpp"
#include "../includes/Asuna/Geometry.hpp"
#include "../includes/Asuna/Camera.hpp"
#include "../includes/Asuna/Transform.hpp"
#include "../includes/Asuna/Input.hpp"


int main(int argc, const char * argv[]) {
    Display display(glm::vec2(1000,700), "3D QuickHull");
    Camera camera(glm::vec3(-200,-300,200), 70.0f, *display.getAspectRatio(), 0.0f, 2000.0f);

    SDL_Quit();
    return 0;
}
