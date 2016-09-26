//
//  QuickHull.cpp
//  Convex Hull
//
//  Created by Sam Haves on 2016-01-02.
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#include "../../includes/Asuna/Geometry/QuickHull.hpp"
#include <stack>
#include <unordered_map>

namespace Asuna{

long int key = 0;

QuickHull::QuickHull(vec3* points, unsigned int numVertices) : m_numVertices(numVertices), m_pointSet(points)
{
    m_mesh = std::make_shared<Mesh>(GL_TRIANGLES, DRAW_INDEXED);
    /* not all points will be vertices on final mesh so final mesh will be created procedurally*/
}

QuickHull::~QuickHull()
{
    //no - op
}

void QuickHull::genHull()
{
    makePyramid();
    /*HE_Vert* maxes = getMax(m_vertices, m_numVertices);
    std::pair<HE_Vert, HE_Vert> mostDist = mostDistantPoints(maxes, 6);

    HE_Vert thirdPoint = mostDistantFromLine(maxes, 6, mostDist.first, mostDist.second);

    auto edge1 = std::make_shared<HalfEdge>();
    auto edge2 = std::make_shared<HalfEdge>();
    auto edge3 = std::make_shared<HalfEdge>();
    edge1->origin = std::make_shared<HE_Vert>(mostDist.first);
    edge2->origin = std::make_shared<HE_Vert>(mostDist.second);
    edge3->origin = std::make_shared<HE_Vert>(thirdPoint);

    auto triBase = std::make_shared<HE_Face>();
    triBase = makeTriFace(edge1, edge2, edge3);
    auto apex = std::make_shared<HE_Vert>(mostDistantFromPlane(m_vertices, m_numVertices, *triBase));

    m_hullFaces = makePyramid(triBase, apex);
    //assign pyramid apex outgoing edge half edges pairs
    std::shared_ptr<HalfEdge> edge = triBase->edge;
    for(unsigned int i= 0; i < 3; i++){
        edge->pair->previous->pair = edge->next->pair->next;
        edge->next->pair->next->pair = edge->pair->previous;
        edge = edge->next;
    }

    for(std::vector<HE_Face>::iterator iter = m_hullFaces.begin(); iter != m_hullFaces.end(); ++iter){
        facePoints.insert(std::pair<long int, std::vector<HE_Vert> >(iter->id, std::vector<HE_Vert>()));
    }

    bool *assigned = new bool[m_numVertices];
    for(int i = 0; i < m_numVertices; i++){
        assigned[i] = false;
    }
    for(std::vector<HE_Face>::iterator iter = m_hullFaces.begin(); iter != m_hullFaces.end(); ++iter){
        glm::vec3 normal  = iter->normal;
        for(unsigned int i =  0; i < m_numVertices; i++){
            glm::vec3 pos = m_vertices[i].position;
            //bad fix here
            if(!assigned[i] && glm::dot(normal, pos- iter->edge->origin->position) > 0 && (pos != apex->position && pos != mostDist.first.position && pos != mostDist.second.position && pos != thirdPoint.position)){
                assigned[i] = true;
                facePoints[iter->id].push_back(m_vertices[i]);
            }
        }
    }
    delete [] assigned;
    //iterateHull();*/
}

void QuickHull::extrude(const sp<HE_Face> base)
{
    vector<unsigned int> points = m_vertSets[base];
    int farthest = -1;
    double distance = 0;
    for(int i = 0; i < points.size(); i++)
    {
      double curDist = getDistanceFromPlane(m_pointSet[points[i]], m_pointSet[base->edge->origin], base->normal);
      if(curDist > distance)
      {
        distance = curDist;
        farthest = i;
      }
    }
    sp<HalfEdge> baseEdge = base->edge;
    do {
      auto edge = baseEdge->pair->next;
      auto face = edge->face;
      if(getDistanceFromPlane(m_pointSet[points[farthest]], m_pointSet[edge->origin], face->normal) > 0)
      {
        auto nF1 = makeTriFace();
        auto nF2 = makeTriFace();
        nF1->edge->pair = edge->pair;
        edge->pair->pair = nF1->edge;
        edge = edge->next;
        nF2->edge->pair = edge->pair;
        edge->pair->pair = nF1->edge;
        //normals!
      }
    } while(baseEdge != base->edge);
    /*
    unsigned int j = 0;
    for(unsigned int i = 0; i < verts.size(); i++){
        auto edge1 = std::make_shared<HalfEdge>();
        auto edge2 = std::make_shared<HalfEdge>();
        auto edge3 = std::make_shared<HalfEdge>();
        edge1->origin = verts[i];
        edge1->pair = pairs[j];
        pairs[j]->pair = edge1;
        edge2->origin = (i == verts.size() - 1) ? verts[0]:verts[i+1];
        edge3->origin = std::make_shared<HE_Vert>(apex);
        auto face = makeTriFace(edge1, edge2, edge3);
        rVec.push_back(face);
        facePoints.insert(std::pair<long int, std::vector<HE_Vert> >(face->id, std::vector<HE_Vert>()));
        j++;
    }
    for(unsigned int i = 0; i < rVec.size(); i++){
        rVec[i]->edge->next->pair = (i == rVec.size() - 1) ? rVec[0]->edge->previous:rVec[i+1]->edge->previous;
        rVec[i]->edge->previous->pair = (i == 0) ? rVec[rVec.size() - 1]->edge->next:rVec[i-1]->edge->next;
    }
    return rVec;*/
}
std::stack<sp<HE_Face> > faces;
/*void QuickHull::iterateHull()
{
    for(vector<HE_Face>::iterator iter = m_hullFaces.begin(); iter != m_hullFaces.end(); ++iter){
        if(facePoints[iter->id].size() > 0) faces.push(std::make_shared<HE_Face>(*iter));
    }
    m_hullFaces.clear();
    while(!faces.empty()){
        auto face = faces.top();
        faces.pop();
        auto farthestPoint = std::make_shared<HE_Vert>(mostDistantFromPlane(&facePoints[face->id][0], facePoints[face->id].size(), *face));
        auto edge = face->edge;
        std::vector<std::shared_ptr<HE_Vert> > extrudeVertices;
        std::vector<std::shared_ptr<HalfEdge> > pairs;
        do{
            extrudeVertices.push_back(edge->origin);
            if(glm::dot(edge->pair->face->normal, farthestPoint->position - edge->pair->origin->position) > 0){
                pairs.push_back(edge->pair->next->pair);
                pairs.push_back(edge->pair->previous->pair);
                extrudeVertices.push_back(edge->pair->previous->origin);
                //facePoints.erase(edge->pair->face->id);
            }else{
                pairs.push_back(edge->pair);
            }
            edge = edge->next;
        }while(edge != face->edge);

        std::vector<std::shared_ptr<HE_Face> > extrudedFaces = extrudeToPoint(*farthestPoint, extrudeVertices, pairs);

        bool *assigned = new bool[facePoints[face->id].size()];
        for(int i = 0; i < facePoints[face->id].size(); i++){
            assigned[i] = false;
        }
        unsigned int i = 0;
        for(std::vector<std::shared_ptr<HE_Face> >::iterator iter2 = extrudedFaces.begin(); iter2 != extrudedFaces.end(); ++iter2){
            std::shared_ptr<HE_Face> curFace = *iter2;
            std::cout << "NUMBER OF NEW FACES: "<< extrudedFaces.size()<< std::endl;
            glm::vec3 normal = curFace->normal;
            for(std::vector<HE_Vert>::iterator iter = facePoints[face->id].begin(); iter != facePoints[face->id].end(); ++iter){
                glm::vec3 pos = iter->position;
                if(!assigned[i] && glm::dot(normal, pos - curFace->edge->origin->position) > 0 && pos != farthestPoint->position){
                    facePoints[curFace->id].push_back(*iter);
                    assigned[i] = true;
                    i++;
                }
            }
            if(facePoints[curFace->id].size() > 0){
                std::cout << "point set not empty" << std::endl;
                faces.push(curFace);
            }else{
                m_hullFaces.push_back(*curFace);
            }
        }
        facePoints.erase(face->id);
    }
    facePoints.clear();
}*/

sp<HE_Face> QuickHull::makeTriFace(){
    auto face = std::make_shared<HE_Face>(vec3(), nullptr);
    sp<HalfEdge> he1 = std::make_shared<HalfEdge>(nullptr, nullptr, nullptr, -1, face);
    sp<HalfEdge> he2 = std::make_shared<HalfEdge>(nullptr, he1, nullptr, -1, face);
    sp<HalfEdge> he3 = std::make_shared<HalfEdge>(he1, he2, nullptr, -1, face);
    face->edge = he1;
    he1->next = he2;
    he1->previous = he3;
    he2->previous = he1;
    return face;
}

void QuickHull::makePyramid(){
  std::cout << "Making pyramid?" << std::endl;
    unsigned int maxes[6] = {0};
    for(int i = 0; i < m_numVertices; i++)
    {
      if(m_pointSet[i].x < m_pointSet[maxes[0]].x) maxes[0] = i;
      if(m_pointSet[i].y < m_pointSet[maxes[1]].y) maxes[1] = i;
      if(m_pointSet[i].z < m_pointSet[maxes[2]].z) maxes[2] = i;
      if(m_pointSet[i].x > m_pointSet[maxes[3]].x) maxes[3] = i;
      if(m_pointSet[i].y > m_pointSet[maxes[4]].y) maxes[4] = i;
      if(m_pointSet[i].z > m_pointSet[maxes[5]].z) maxes[5] = i;
    }
    //Find the two farthest points in the set
    double distance = 0.0;
    pair<unsigned int, unsigned int> mostDist;
    for(int i = 0; i < 6; i++)
    {
      for(int j = 0; j < 6; j++)
      {
        if (i == j) continue;
        if(glm::distance(m_pointSet[maxes[i]], m_pointSet[maxes[j]]) > distance){
            distance = glm::distance(m_pointSet[maxes[i]], m_pointSet[maxes[j]]);
            mostDist = pair<unsigned int, unsigned int>(maxes[i], maxes[j]);
        }
      }
    }

    //Build the largest triangle possible with those verts
    distance = 0.0;
    unsigned int farthestFromLine;
    vec3 p1 = m_pointSet[mostDist.first], p2 = m_pointSet[mostDist.second];
    for(int i = 0; i < 6; i++)
    {
        if(maxes[i] == mostDist.first || maxes[i] == mostDist.second) continue;
        vec3 p3 = m_pointSet[maxes[i]];
        if(getDistanceFromLine(p3, p1, p2) > distance)
        {
            distance = getDistanceFromLine(p3, p1, p2);
            farthestFromLine = maxes[i];
        }
    }

    //These are going to be part of the hull so add them to mesh.
    //Probably add some default texture coords when I get around to adding support for passing UV function to meshes
    m_mesh->addVertex(new Vertex(m_pointSet[mostDist.first], vec2(0,0)));
    m_mesh->addVertex(new Vertex(m_pointSet[mostDist.second], vec2(0,0)));
    m_mesh->addVertex(new Vertex(m_pointSet[farthestFromLine], vec2(0,0)));
    //construct the half edges and face
    vec3 normal = glm::normalize(glm::cross(m_pointSet[mostDist.first] - m_pointSet[mostDist.second], m_pointSet[farthestFromLine] - m_pointSet[mostDist.second]));
    sp<HE_Face> face = std::make_shared<HE_Face>(normal, nullptr);
    sp<HalfEdge> edge1 = std::make_shared<HalfEdge>(nullptr, nullptr, nullptr, 0, face);
    sp<HalfEdge> edge2 = std::make_shared<HalfEdge>(nullptr, edge1, nullptr, 1, face);
    sp<HalfEdge> edge3 = std::make_shared<HalfEdge>(edge1, edge2, nullptr, 2, face);
    //edges form internal loop so it doensnt matter which one we assign to the face
    face->edge = edge1;
    //filling in missing fields. Need to assign pairings later when pyramid is formed.
    edge1->next = edge2; edge1->previous = edge3;
    edge2->next = edge3;
    m_hullFaces.push_back(face);

    //Find the farthest point from the triangle plane, this searches all points in point set
    unsigned int farthestFromPlane = mostDistantFromPlane(vec3(mostDist.first, mostDist.second, farthestFromLine), m_pointSet[farthestFromLine], normal);
    std::cout << mostDist.first << " " << mostDist.second << " " << farthestFromLine << " " << farthestFromPlane << std::endl;
    m_mesh->addVertex(new Vertex(m_pointSet[farthestFromPlane], vec2(0,0)));
    sp<HalfEdge> baseEdge = edge1;

    do
    {
      vec3 normal = glm::normalize(glm::cross(m_pointSet[farthestFromPlane] - m_pointSet[baseEdge->origin], m_pointSet[baseEdge->next->origin] - m_pointSet[baseEdge->origin]));
      auto curFace = std::make_shared<HE_Face>(normal, nullptr);
      sp<HalfEdge> e1 = std::make_shared<HalfEdge>(nullptr, nullptr, nullptr, baseEdge->next->origin, curFace);
      sp<HalfEdge> e2 = std::make_shared<HalfEdge>(nullptr, e1, nullptr, baseEdge->origin, curFace);
      sp<HalfEdge> e3 = std::make_shared<HalfEdge>(e1, e2, nullptr, farthestFromPlane, curFace);
      curFace->edge = e1;
      //filling in missing fields. Need to assign pairings later when pyramid is formed.
      e1->next = e2; e1->previous = e3;
      e2->next = e3;
      baseEdge->pair = e1;
      baseEdge = baseEdge->next;
      m_hullFaces.push_back(curFace);
    }while(baseEdge != edge1);
    //pairing up half edges
    for(int i = 0; i < 2; i++)
    {
      baseEdge->pair->previous->pair = baseEdge->next->pair->next;
      baseEdge->next->pair->next->pair = baseEdge->pair->previous;
    }
    vector<bool> assigned;
    assigned.reserve(m_numVertices);
    for(auto face: m_hullFaces)
    {
      for(int i = 0; i < m_numVertices; i++)
      {
        if(!assigned[i] && getDistanceFromPlane(m_pointSet[i], m_pointSet[face->edge->origin], face->normal) > 0)
        {
          assigned[i] = true;
          m_vertSets[face].push_back(i);
        }
      }
    }

    m_mesh->addTriangle(0,1,2);
    m_mesh->addTriangle(0,2,3);
    m_mesh->addTriangle(0,1,3);
    m_mesh->addTriangle(1,2,3);
    m_mesh->mapBuffers();

}

double QuickHull::getDistanceFromLine(const vec3& point, const vec3& p1, const vec3& p2){
    return glm::length(glm::cross(p2 - p1, p1 - point))/ glm::length(p2 - p1);
}

double QuickHull::getDistanceFromPlane (const vec3& point, const vec3& pointOnPlane , const vec3& normal)
{
    glm::vec3 pointFromPlane = point - pointOnPlane;
    return glm::length((glm::dot(pointFromPlane, normal)/glm::length(normal)) * normal);
}

unsigned int QuickHull::mostDistantFromPlane(const vec3& baseIndices, const vec3& pointOnPlane, const vec3& normal)
{
    double distance = 0;
    unsigned int rIndex = -1;
    for(int i = 0; i < m_numVertices; i++)
    {
      if(i == baseIndices.x || i == baseIndices.y || i == baseIndices.z) continue;
      double curDistance = getDistanceFromPlane(m_pointSet[i], pointOnPlane , normal);
      if(curDistance > distance)
      {
        distance = curDistance;
        rIndex = i;
      }
    }
    return rIndex;
}
}//Asuna namespace
