//
//  main.cpp
//  Asuna
//  Created By Sam Haves
//


#include <iostream>
#include <vector>
#include <random>
#include <GL/glew.h>

#include "../includes/Asuna/Asuna.hpp"

template<typename T>
using sp = std::shared_ptr<T>;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
template<typename T>
using vector = std::vector<T>;


int main(int argc, const char * argv[]) {
    /*********inits**************/
    Asuna::Display display(vec2(1000,700), "3D QuickHull");
    Asuna::Shader shader("basicShader");
    Asuna::Input input;
    Asuna::Transform transform;
    Asuna::Camera camera(vec3(-200,-300,200), 70.0f, display.getAspectRatio(), 0.0f, 2000.0f,vec3(200,300,-200));
    Asuna::Sphere sphere(vec3(0,0,0), 100.0f);

    /**********setup************/
    sphere.genMesh();
    vector<sp<Asuna::Mesh>> meshes;
    meshes.push_back(sphere.getMesh());

    /***********'draw loop'**********/
    while(!display.isClosed()){

      display.clear(1.0f, 1.0f, 1.0f, 1.0f);
      shader.bind();
      input.applyKeyPresses(transform, camera);
      shader.update(transform, camera);
      sphere.getMesh()->draw();
      for(int i = 0; i < meshes.size(); i++){
          meshes[i]->draw();
      }
      display.update();
    }
    return 0;
}
