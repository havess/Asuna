//
//  Camera.hpp
//  OpenGLTut
//
//  Created by Sam Haves on 2015-12-24.
//  Copyright © 2015 Sam Haves. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class Camera{
public:
    Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar, const glm::vec3& forward = glm::vec3(0,0,1), const glm::vec3& up = glm::vec3(0,1,0)){
        m_perspective = glm::perspective(fov, aspect, zNear, zFar);
        m_position = pos;
        m_forward = -pos;
        m_up = up;
    }
    
    inline glm::mat4 getViewProjection() const{
        return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
    }
    
    inline glm::vec3* getForward(){return &m_forward;};
    
protected:
private:
    glm::mat4 m_perspective;
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_up;
    
};

#endif /* Camera_hpp */
