#pragma once

#include <iostream>

#include "../Geometry/Sphere.hpp"

/*
This is an interface for generating demos that will be callable from the console in the hud, this would reduce the
number of needed macros for testing and/or quitting, changing parameters, compiling, etc. just to render somehting new
This will likely be a very long file eventually.
*/

template<typename T>
using sp = std::shared_ptr<T>;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
template<typename T>
using vector = std::vector<T>;
template<typename T, typename K>
using map = std::map<T,K>;

enum DemoEnum{
  BASIC_DEMO,
  SPHERE_DEMO,
  DEMO_COUNT
};


namespace Asuna{


struct DemoTemplate{
public:

  inline DemoEnum getType(){return type;}
  virtual bool init() = 0;
  virtual bool shutDown() = 0;
  virtual void execute() = 0;
protected:
  DemoTemplate(DemoEnum type): type(type){}
  virtual ~DemoTemplate(){}
private:
  DemoEnum type;
};

struct BasicDemo: public DemoTemplate
{
public:
  BasicDemo(const vec3 position = vec3()) : m_position(position), DemoTemplate(BASIC_DEMO){}
  virtual bool init()
  {
     m_mesh = std::make_shared<Mesh>(Mesh(GL_LINES, DRAW_BASIC));
     if (m_mesh == nullptr) return false;
     printf("Initializing basic demo\n");
     m_mesh->setOffset(m_position);
     m_mesh->addVertex(new Vertex(vec3(0,0,0), vec2(0,0)));
     m_mesh->addVertex(new Vertex(vec3(1,0,1), vec2(0,0)));
     m_mesh->addVertex(new Vertex(vec3(0,0,1), vec2(0,0)));
     m_mesh->mapBuffers();
     return true;
  }

  virtual bool shutDown()
  {
    return true;
  }

  virtual void execute()
  {
    m_mesh->draw();
  }
private:
  vec3     m_position;
  sp<Mesh> m_mesh;
};

struct SphereDemo: public DemoTemplate{
public:
  SphereDemo(const vec3 position = vec3() ,const float radius = 50.0f) : DemoTemplate(SPHERE_DEMO){
    m_sphere = new Sphere(position , radius);
  }
  virtual ~SphereDemo(){shutDown();}
  virtual bool init(){
    //initialize DemoHandler
    m_sphere->genMesh();
    return true;
  }
  virtual bool shutDown(){
    delete m_sphere;
    return true;
  }
  virtual void execute(){
    //execute method called every update
    m_sphere->render();
  }
private:
  Asuna::Sphere* m_sphere;
};

class DemoHandler{
public:
  DemoHandler(){}
  virtual ~DemoHandler(){}

  bool init(DemoEnum type){
    if(type > SPHERE_DEMO) {return false;}
    if(!demos.count(type)){
      demos[type] = getDemo(type);
    }
    demos[type]->init();
    return true;
  }
  bool shutDown(DemoEnum type){
    if(!demos.count(type)){return false;}
    demos.erase(type);
    return true;
  }
  void execute(){
    for(std::pair<DemoEnum,DemoTemplate*> p: demos){
      p.second->execute();
    }
  }
private:
  DemoTemplate* getDemo(DemoEnum type){
    switch(type){
      //specify parameters if you want it to be non default
      case SPHERE_DEMO:
          return new SphereDemo(vec3(0,0,0), 75.0f);
      case BASIC_DEMO:
          return new BasicDemo();
      default: std::cerr << "Error: the specified DemoEnum is invalid" << std::endl;
    }
    return nullptr;
  }
  map<DemoEnum, DemoTemplate*> demos;
};
}
