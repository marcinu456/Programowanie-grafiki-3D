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

    // This is a set of vertices positions and colors
    // The layout is described in the vertex shader
    std::vector<GLfloat> vertices =
        {
        //floor of pyramid
        -0.5f,  0.0f,   -0.5f,  0.0f, 0.0f, 1.0f,//0
        -0.5f,  0.0f,   0.5f,   0.0f, 0.0f, 1.0f,//1
        0.5f,   0.0f,   -0.5f,  0.0f, 0.0f, 1.0f,//2
        0.5f,   0.0f,   0.5f,   0.0f, 0.0f, 1.0f,//3
        //front wall
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,//4
        0.0f, 1.f, 0.0f, 0.0f, 1.0f, 0.0f,//5
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,//6
        //right wall
        0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f,//7
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//8
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,//9
        //back wall
        -0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f,//10
        0.f, 1.f, 0.f, 0.0f, 0.5f, 1.0f,//11
        0.5f, 0.f, 0.5f, 0.0f, 0.5f, 1.0f,//12
        //left wall
        -0.5f, 0.f, 0.5f, 1.0f, 1.0f, 0.0f,//13
        0.f, 1.f, 0.f, 1.0f, 1.0f, 0.0f,//14
        -0.5f, 0.f, -0.5f,1.0f, 1.0f, 0.0f,//15
        };

    std::vector<GLuint> indices =
        {
        //floor of pyramid
        0, 2, 1,
        1, 2, 3,
        //front wall
        4, 5, 6,
        //right wall
        7, 8, 9,
        //back wall
        11, 10, 12,
        //left wall
        13, 14, 15,
        };
    
    
    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //generating indices buffer object
    GLuint idx_buffer_handle;
    glGenBuffers(1, &idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);


    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(0);
    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBindVertexArray(0);
    //end of vao "recording"

    Uniform();
    SetCameraPVM();

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
    
    glm::mat4 PVM = camera_->GetPVM();
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    //glDrawArrays(GL_TRIANGLES, 0, 9);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);

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