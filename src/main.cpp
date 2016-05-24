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


int main(int argc, const char * argv[]) {
    /*********inits**************/
    Asuna::Display display(vec2(1000,700), "3D QuickHull", true);
    Asuna::Shader shader("basicShader");
    Asuna::Input input;
    Asuna::DemoHandler demoHandler;

    Asuna::Transform transform;
    Asuna::Camera camera(vec3(-200,-300,200), 70.0f, display.getAspectRatio(), 0.0f, 2000.0f,vec3(200,300,-200));

    /**********setup************/
    demoHandler.init(SPHERE_DEMO);

    /***********'draw loop'**********/
    while(!display.isClosed()){

      display.clear(1.0f, 1.0f, 1.0f, 1.0f);
      shader.bind();
      input.applyKeyPresses(transform, camera);
      shader.update(transform, camera);
      demoHandler.execute();
      display.update();
    }
    return 0;
}
