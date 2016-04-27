//
//  QuickHull.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include <assert.h>
#include <vector>
#include "Mesh.hpp"

namespace Asuna{

class QuickHull{
public:
    QuickHull(glm::vec3* points, int numVertices);
    virtual ~QuickHull();
    void genHull();
    void iterateHull();
    std::shared_ptr<Mesh> getHull();

private:
    unsigned long m_numVertices;

    std::vector<HE_Face> m_hullFaces;
    HE_Vert* m_vertices;

};
}
