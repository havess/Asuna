//
//  Transform.hpp
//  Asuna
//
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

typedef glm::mat4 mat4;
typedef glm::vec3 vec3;

namespace Asuna{

class Transform{
public:

    Transform(const vec3& pos = vec3(), const vec3& rot  = vec3(), const vec3& scale  = vec3(1.0f, 1.0f, 1.0f)): m_pos(pos),m_rot(rot),m_scale(scale) {}

    inline mat4 getModel() const{
        mat4 posMatrix = glm::translate(m_pos);
        mat4 rotXMatrix = glm::rotate(m_rot.x, vec3(1,0,0));
        mat4 rotYMatrix = glm::rotate(m_rot.y, vec3(0,1,0));
        mat4 rotZMatrix = glm::rotate(m_rot.z, vec3(0,0,1));
        mat4 scaleMatrix = glm::scale(m_scale);

        mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

        return posMatrix * rotMatrix * scaleMatrix;
    }

    inline vec3& getPos(){return m_pos;}
    inline vec3& getRot(){return m_rot;}
    inline vec3& getScale(){return m_scale;}

    inline void setPos(const vec3& pos){m_pos = pos;}
    inline void setRot(const vec3& rot){m_rot = rot;}
    inline void setScale(const vec3& scale){m_scale = scale;}
protected:
private:
    vec3 m_pos;
    vec3 m_rot;
    vec3 m_scale;
};
}
