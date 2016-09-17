//
//  QuickHull.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#pragma once

#include <iostream>
#include <assert.h>
#include "Mesh.hpp"
#include "Common.hpp"

namespace Asuna{

class QuickHull{
public:
    QuickHull(vec3* points, unsigned int numVertices);
    virtual ~QuickHull();
    void genHull();
    inline sp<Mesh> getHull() { return m_mesh; }

private:
    unsigned long                    m_numVertices;
    sp<Mesh>                         m_mesh;
    vector<sp<HE_Face>>              m_hullFaces;
    vec3*                            m_pointSet;
    map<sp<HE_Face>, vector<unsigned int>> m_vertSets;

    double getDistanceFromPlane(const vec3& point, const vec3& pointOnPlane , const vec3& normal);
    double getDistanceFromLine(const vec3& point, const vec3& p1, const vec3& p2);
    unsigned int mostDistantFromPlane(const vec3& baseIndices, const vec3& pointOnPlane, const vec3& normal);
    void makeTriFace(std::shared_ptr<HalfEdge> he1, std::shared_ptr<HalfEdge> he2, std::shared_ptr<HalfEdge> he3);
    void makePyramid();
    void extrude(const sp<HE_Face> base);

};
}
