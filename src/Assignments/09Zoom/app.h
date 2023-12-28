//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"
#include <glm/glm.hpp>

#include "camera.h"

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override; 

    void scroll_callback(double xoffset, double yoffset) override;
private:

    void Uniform();

    void SetCameraPVM();
    
    GLuint vao_;
    
    GLuint u_pvm_buffer_;

    //Camera
    Camera *camera_;

    //Set camera
    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }

    
};
