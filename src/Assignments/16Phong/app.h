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
#include "camera_controler.h"
#include "Engine/Mesh.h"
#include "Engine/Material.h"


class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title,
        debug), m_color_material(glm::vec4{ 0.7f, 0.7f, 0.7f, 1.0f })
    {
    }

    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
        if (camera_controler_) {
            delete camera_controler_;
        }
        for (auto mesh : meshes_) {
            delete mesh;
        }
    }

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override; 

    void scroll_callback(double xoffset, double yoffset) override;

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;

    void key_callback(int key, int scancode, int action, int mods) override;
private:

    void Uniform();

    void SetCameraPVM();
    
    GLuint u_pvm_buffer_;

    //Camera
    Camera *camera_;

    //Set camera
    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }

    //Camera Controler
    CameraControler *camera_controler_;
    void set_camera_controler(CameraControler *camera_controler) { camera_controler_ = camera_controler; }

    //Meshes
    void SetPyramidMeshes(std::vector<GLfloat> &vertices, std::vector<GLushort> indices);

    //Textures
    GLuint SetTextures();
    
    std::vector<xe::Mesh*> meshes_;
    void add_submesh(xe::Mesh *mesh) {
        meshes_.push_back(mesh);
    }

    //Materials
    xe::ColorMaterial m_color_material;

    std::shared_ptr<xe::Mesh> mesh;
};
