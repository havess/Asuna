//
//  main.cpp
//  Asuna
//  Created By Sam Haves
//


#include <iostream>
#include <vector>
#include <random>


#include "../includes/Asuna/Asuna.hpp"

template<typename T>
using sp = std::shared_ptr<T>;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
template<typename T>
using vector = std::vector<T>;

int main(int argc, const char * argv[])
{
    /*********inits**************/
    Asuna::Display display(vec2(1000,700), "3D QuickHull", true);
    Asuna::Input input;
    Asuna::DemoHandler demoHandler;

    Asuna::Transform transform;
    Asuna::Camera camera(vec3(-200,-300,200), 70.0f, display.getAspectRatio(), 0.0f, 2000.0f, vec3(200,300,-200));

    /**********setup************/
    Asuna::ProgramObject pObj;
    pObj.createVertexShader("./shaders/basicShader.vs");
    pObj.bindAttribLocation(0, "position");
    pObj.bindAttribLocation(1, "texCoord");
    pObj.createFragmentShader("./shaders/basicShader.fs");
    pObj.link();
    demoHandler.init(QUICKHULL_DEMO);

    /***********'draw loop'**********/
    while(!display.isClosed())
    {
      display.clear(0.5f, 0.6f, 0.55f, 1.0f);
      pObj.bind();
      input.applyKeyPresses(transform, camera);
      pObj.update(transform, camera);
      demoHandler.execute();
      //this needs to be called last
      display.update();
    }
    return 0;
}
