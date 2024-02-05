//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <vector>
#include <tuple>
#define _WIN32_WINNT  0x0501 // I misunderstand that
#include <windows.h>
#include <iostream>
#include <cstdlib>

#include <glm/gtc/type_ptr.hpp>


#include "Application/utils.h"
#include "Engine/Mesh.h"
#include "Engine/ColorMaterial.h"
#include "Engine/PhongMaterial.h"
#include "Engine/mesh_loader.h"

#define STB_IMAGE_IMPLEMENTATION  1

#include "3rdParty/stb/stb_image.h"


void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
            {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
             {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    xe::ColorMaterial::init();
    xe::PhongMaterial::init();
    
    mesh = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj",
                                        std::string(ROOT_DIR) + "/Models");
    add_submesh(mesh.get());

    SetCameraPVM();

    //light
    auto light = new xe::PointLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.09f);
    auto light1 = new xe::PointLight(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f), 1.0f, 0.09f);
    add_light(*light);
    add_light(*light1);

    glGenBuffers(1, &Matrixes);
    glBindBuffer(GL_UNIFORM_BUFFER, Matrixes);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

    glGenBuffers(1, &Lights);
    glBindBuffer(GL_UNIFORM_BUFFER, Lights);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightsArray), nullptr, GL_STATIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, 2, Matrixes);
    glBindBuffer(GL_UNIFORM_BUFFER, Matrixes);

    glBindBufferBase(GL_UNIFORM_BUFFER, 3, Lights);
    glBindBuffer(GL_UNIFORM_BUFFER, Lights);

    // Set ambient light
    xe::PhongMaterial::set_ambient(glm::vec3(0.1f, 0.1f, 0.1f));
    
    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto[w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(program);


}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame()
{
    m_color_material.bind();
    glm::mat4 PVM = camera_->GetPVM();
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    auto vm = camera_->view() * camera_->get_model();
    auto R = glm::mat3(vm);
    auto N = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1]));

    MatrixesArray matricesBlock{};

    matricesBlock.N = N;
    matricesBlock.VM = vm;

    glBindBuffer(GL_UNIFORM_BUFFER, Matrixes);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MatrixesArray), &matricesBlock);

    glBindBuffer(GL_UNIFORM_BUFFER, Lights);

    LightsArray lightBlock{};

    lightBlock.num_lights = p_lights_.size();

    for(int i = 0; i < p_lights_.size(); i++) {
        lightBlock.light[i] = p_lights_[i];
    }

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightsArray), &lightBlock);
    
    // MeshesMaterials
    for (auto m: meshes_) {
        m->draw();
    }

}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h)
{
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    if(camera_)
        camera_->set_aspect((float)w / h);
    //aspect_ = (float) w / h;
    //Projection = glm::perspective(fov_, aspect_, near_, far_);
    
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset)
{
    Application::scroll_callback(xoffset, yoffset);
    if(camera_)
        camera_->zoom(yoffset/30);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods)
{
    Application::mouse_button_callback(button, action, mods);

    if (camera_controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            camera_controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            camera_controler_->LMB_released(x, y);
    }
}

void SimpleShapeApplication::cursor_position_callback(double x, double y)
{
    Application::cursor_position_callback(x, y);
    if (camera_controler_) {
        camera_controler_->mouse_moved(x, y);
    }
}

void SimpleShapeApplication::key_callback(int key, int scancode, int action, int mods)
{
    Application::key_callback(key, scancode, action, mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
    }
}

void SimpleShapeApplication::Uniform()
{
    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    
    float strength=.25;
    float color[3] ={1.5f,1.2f,1.1f};

    glGenBuffers(1, &v_buffer_handle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, v_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 3*sizeof(float), 3*sizeof(float), color);
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SimpleShapeApplication::SetCameraPVM()
{
    //Setup Camera
    set_camera(new Camera(glm::pi<float>() / 4.0,
        static_cast<float>(frame_buffer_size().first)/static_cast<float>(frame_buffer_size().second),
        0.1f,
        100.0f));

    camera_->look_at(glm::vec3(0.0f, 10.f, 15.f), // Camera is at (0,-10,-10), in World Space
        glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
        glm::vec3(0.f, 1.f, 0.f)); // Head is up (set to 0,-1,0 to look upside-down)
    
    //Generating PVM buffer
    glGenBuffers(1, &u_pvm_buffer_);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Init camera controler
    set_camera_controler(new CameraControler(camera_));
}

void SimpleShapeApplication::SetPyramidMeshes(std::vector<GLfloat> &vertices, std::vector<GLushort> indices)
{
    //meshes
    auto MeshPyramid_ = new xe::Mesh;
    MeshPyramid_->allocate_vertex_buffer(vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    MeshPyramid_->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());
    MeshPyramid_->vertex_attrib_pointer(0, 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
    MeshPyramid_->vertex_attrib_pointer(1, 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    MeshPyramid_->allocate_index_buffer(indices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    MeshPyramid_->load_indices(0, indices.size() * sizeof(GLfloat), indices.data());

    auto Texture = SetTextures();
    
   // MeshPyramid_->add_submesh(0, 3, new xe::ColorMaterial({1.0f, 0.0f, 0.0f, 1.0f},Texture)  );
  //  MeshPyramid_->add_submesh(3, 6, new xe::ColorMaterial({0.0f, 1.0f, 0.0f, 1.0f},Texture)  );
  //  MeshPyramid_->add_submesh(6, 9, new xe::ColorMaterial({0.0f, 0.0f, 1.0f, 1.0f},Texture)  );
  //  MeshPyramid_->add_submesh(9, 12, new xe::ColorMaterial({1.0f, 1.0f, 0.0f, 1.0f},Texture)  );
  //  MeshPyramid_->add_submesh(12, 18, new xe::ColorMaterial({0.0f, 1.0f, 1.0f, 1.0f},Texture)  );
    add_submesh(MeshPyramid_);

    
}

GLuint SimpleShapeApplication::SetTextures()
{
    stbi_set_flip_vertically_on_load(true);
    GLint width, height, channels;
    auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
    auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    if (!img) {
        //spdlog::warn("Could not read image from file `{}'", texture_file);
        std::cerr << "Could not read image from file `{}'" << texture_file << std::endl;
    }
    //textures
    GLuint texture_ = 0;
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);



    
    return texture_;
}
