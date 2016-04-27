//
//  Camera.hpp
//  Asuna
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

namespace Asuna
{

class Camera{
public:
    Camera(): m_perspective(0), m_position(0), m_forward(0), m_up(0){}
    Camera(const vec3& pos, float fov, float aspect, float zNear, float zFar, const vec3& forward = vec3(0,0,1), const vec3& up = vec3(0,1,0)):
            m_perspective(glm::perspective(fov, aspect, zNear, zFar)), m_position(pos), m_forward(forward), m_up(up) {}

    inline mat4 getViewProjection() const{
        return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
    }

    inline vec3 getForward() const{return m_forward;};

protected:
private:
    mat4 m_perspective;
    vec3 m_position;
    vec3 m_forward;
    vec3 m_up;

};
}
