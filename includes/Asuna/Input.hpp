//
//  Input.hpp
//  Asuna
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include "Transform.hpp"
#include "Camera.hpp"

namespace Asuna{

class Input{
public:
    void applyKeyPresses(Transform& transform, Camera& camera){

        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if(state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_SPACE]){
            transform.getRot().z += 0.05f;
        }else if (state[SDL_SCANCODE_RIGHT]) {
            transform.getRot().y += 0.05f;
        }
        if(state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_SPACE]){
            transform.getRot().z -= 0.05f;
        }else if (state[SDL_SCANCODE_LEFT]) {
            transform.getRot().y -= 0.05f;
        }
        if (state[SDL_SCANCODE_DOWN]) {
            transform.getRot().x -= 0.05f;
        }
        if (state[SDL_SCANCODE_UP]) {
            transform.getRot().x += 0.05f;
        }
        if (state[SDL_SCANCODE_Z]) {
            transform.getPos() += 0.04f * camera.getForward();
        }
        if (state[SDL_SCANCODE_X]) {
            transform.getPos() -= 0.04f * camera.getForward();
        }

    }
};
}
