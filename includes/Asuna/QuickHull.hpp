//
//  QuickHull.hpp
//  Convex Hull
//
//  Created by Sam Haves on 2016-01-02.
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#ifndef QuickHull_hpp
#define QuickHull_hpp

#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <vector>
#include "Mesh.hpp"


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

#endif /* QuickHull_hpp */
