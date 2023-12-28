//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"
#include <glm/glm.hpp>

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override; 

    void Uniform();

    void PVM();

private:
    GLuint vao_;

    float fov_;
    float aspect_;
    float near_;
    float far_; 

    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 Model;

    GLuint u_pvm_buffer_;
};