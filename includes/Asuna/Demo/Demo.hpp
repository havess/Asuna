#pragma once
#include "../Geometry/Geometry.hpp"


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
template<typename T>
using map = std::map<T>

enum DemoEnum{
  ASUNA_DEMO,
  SPHERE_DEMO,
  DEMO_COUNT
}

struct DemoTemplate{
public:
  inline DemoEnum getType(){return type;}
protected:
  virtual DemoTemplate(DemoEnum type): type(type){}
  virtual ~DemoTemplate(){}
  virtual init() = 0;
  virtual shutDown() = 0;
  virtual execute() = 0;
private:
  DemoEnum type;
}

struct SphereDemo: public DemoTemplate{
public:
  SphereDemo(const vec3 position = vec3() ,const float radius = 50.0f):DemoTemplate(SPHERE_DEMO){
    m_sphere = new Sphere(position , radius);
  }
  virtual ~SphereDemo(){shutDown();}
  virtual init(){
    //initialize demo
    m_sphere.genMesh();
  }
  virtual shutDown(){
    //do nothing for now.
  }
  virtual execute(){
    //execute method called every update
    m_sphere.render();
  }
private:
  Sphere m_sphere;
}

class Demo{
public:
  Demo();
  virtual ~Demo();

  bool init(DemoEnum type){

  }
  bool shutDown(DemoEnum type);
  void execute(DemoEnum type);
private:
  map<Demo> demos;
};
