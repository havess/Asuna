//
//  QuickHull.cpp
//  Convex Hull
//
//  Created by Sam Haves on 2016-01-02.
//  Copyright © 2016 Sam Haves. All rights reserved.
//

#include "../includes/Asuna/QuickHull.hpp"
#include <stack>
#include <unordered_map>

double getDistanceFromPlane(const glm::vec3& point, const HE_Vert& vertex, const glm::vec3& normal);
HE_Vert* getMax(HE_Vert* vertices, unsigned long const numVertices);
std::pair<HE_Vert, HE_Vert> mostDistantPoints(HE_Vert vertices[], int numVertices);
HE_Vert mostDistantFromLine(HE_Vert* vertices, int numPairs, HE_Vert& p1, HE_Vert& p2);
HE_Vert mostDistantFromPlane(HE_Vert* vertices, unsigned long const numPairs, HE_Face& face);
std::shared_ptr<HE_Face> makeTriFace(std::shared_ptr<HalfEdge> he1, std::shared_ptr<HalfEdge> he2, std::shared_ptr<HalfEdge> he3);
std::vector<HE_Face> makePyramid(std::shared_ptr<HE_Face> triBase, std::shared_ptr<HE_Vert> apex);
void iterateHull(std::vector<HE_Face>& hullFaces);

long int key = 0;
std::unordered_map<long int, std::vector<HE_Vert> > facePoints;

QuickHull::QuickHull(glm::vec3* points, int numVertices){
    m_vertices = new HE_Vert[numVertices];
    for(unsigned int i  = 0; i < numVertices; i++){
        m_vertices[i].position = points[i];
    }
    m_numVertices = numVertices;
}

QuickHull::~QuickHull(){
    delete[] m_vertices;
}

void QuickHull::genHull(){
    HE_Vert* maxes = getMax(m_vertices, m_numVertices);
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
    //iterateHull();
}

std::vector<std::shared_ptr<HE_Face> > extrudeToPoint(HE_Vert& apex, std::vector<std::shared_ptr<HE_Vert> > verts, std::vector<std::shared_ptr<HalfEdge> > pairs){
    std::vector<std::shared_ptr<HE_Face> > rVec;
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
    return rVec;
}
std::stack<std::shared_ptr<HE_Face> > faces;
void QuickHull::iterateHull(){
    for(std::vector<HE_Face>::iterator iter = m_hullFaces.begin(); iter != m_hullFaces.end(); ++iter){
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

}



std::shared_ptr<Mesh> QuickHull::getHull(){
    std::vector<Vertex> vertices;
    for(std::vector<HE_Face>::iterator iter = m_hullFaces.begin(); iter != m_hullFaces.end(); ++iter){
        std::shared_ptr<HalfEdge> edge = iter->edge;
        for(unsigned int j = 0; j < 3; j++){
            glm::vec3 pos = edge->origin->position;
            vertices.emplace_back(Vertex(pos, glm::vec2(0,0)));
            edge = edge->next;
        }
    }
    return std::shared_ptr<Mesh>(new Mesh(&vertices[0], (uint) vertices.size(), GL_TRIANGLES));
}

std::shared_ptr<HE_Face> makeTriFace(std::shared_ptr<HalfEdge> he1, std::shared_ptr<HalfEdge> he2, std::shared_ptr<HalfEdge> he3){
    auto face = std::make_shared<HE_Face>();
    face->id = key++;
    face->edge = he1;
    he1->next = he2;
    he2->next = he3;
    he3->next = he1;
    he1->previous = he3;
    he2->previous = he1;
    he3->previous = he2;
    face->normal = glm::normalize(glm::cross(he1->next->origin->position - he1->origin->position,  he1->previous->origin->position - he1->origin->position ));
    he1->face = face;
    he2->face = face;
    he3->face = face;
    return face;
}


std::vector<HE_Face> makePyramid(std::shared_ptr<HE_Face> triBase, std::shared_ptr<HE_Vert> apex){
    std::vector<HE_Face> faces;
    std::shared_ptr<HalfEdge> edge = triBase->edge;
    do{
        auto pair = std::make_shared<HalfEdge>();
        auto apexToV1= std::make_shared<HalfEdge>();
        auto v0ToApex= std::make_shared<HalfEdge>();
        pair->origin = edge->next->origin;
        apexToV1->origin = apex;
        v0ToApex->origin = edge->origin;

        //ordering is important, just trust me on this one
        edge->pair = std::shared_ptr<HalfEdge>(pair);
        pair->pair = edge;
        faces.push_back(*makeTriFace(pair, v0ToApex, apexToV1));
        edge = edge->next;

    }while(edge != triBase->edge);
    faces.push_back(*triBase);
    return faces;

}

double getDistanceFromLine(const glm::vec3& point, const glm::vec3& p1, const glm::vec3& p2){
    return glm::length(glm::cross(p2 - p1, p1 - point))/ glm::length(p2 - p1);
}

double getDistanceFromPlane (const glm::vec3& point, HE_Vert& vertex, const glm::vec3& normal){
    glm::normalize(normal);
    glm::vec3 pointFromPlane = point - vertex.position;
    return glm::length((glm::dot(pointFromPlane, normal)/glm::length(normal)) * normal);
}

/* 0 is x, 1 is y, 2 is z
   first is max, second is min*/
HE_Vert* getMax(HE_Vert* vertices, const unsigned long numVertices){
    static HE_Vert rVertices[6];
    for(unsigned int i = 0; i < numVertices; i++){
        HE_Vert cur = vertices[i];
        //maximums
        if(cur.position.x > rVertices[0].position.x) rVertices[0] = cur;
        if(cur.position.y > rVertices[1].position.y) rVertices[1] = cur;
        if(cur.position.z > rVertices[2].position.z) rVertices[2] = cur;
        //std::cout<< numVertices << std::endl;
        //minimums
        if(cur.position.x < rVertices[3].position.x) rVertices[3] = cur;
        if(cur.position.y < rVertices[4].position.y) rVertices[4] = cur;
        if(cur.position.z < rVertices[5].position.z) rVertices[5] = cur;
    }
    return &rVertices[0];
}

std::pair<HE_Vert, HE_Vert> mostDistantPoints(HE_Vert vertices[], const int numPairs){
    double distance = 0;
    std::pair<HE_Vert, HE_Vert> rPair;
    for(int i = 0; i < numPairs; i++){
        if(glm::distance(vertices[i].position, vertices[numPairs - i].position) > distance){
            distance = glm::distance(vertices[i].position, vertices[numPairs - i - 1].position);
            rPair = std::pair<HE_Vert, HE_Vert>(vertices[i], vertices[numPairs - i - 1]);
        }
    }
    return rPair;
}

HE_Vert mostDistantFromLine(HE_Vert* vertices,int numVerts, HE_Vert& p1, HE_Vert& p2){
    double distance = 0;
    HE_Vert rVert;
    for(int i = 0; i < numVerts; i++){
        if(getDistanceFromLine(vertices[i].position, p1.position, p2.position) > distance){
            distance = getDistanceFromLine(vertices[i].position, p1.position, p2.position);
            rVert = vertices[i];
        }
    }
    return rVert;
}

HE_Vert mostDistantFromPlane(HE_Vert* vertices, unsigned long int numVerts, HE_Face& face){
    double distance = 0;
    HE_Vert rVert;
    for(int i = 0; i < numVerts; i++){
        if(getDistanceFromPlane(vertices[i].position, *face.edge->origin , face.normal) > distance){
            distance = getDistanceFromPlane(vertices[i].position, *face.edge->origin , face.normal);
            rVert = vertices[i];
        }
    }
    return rVert;
}
