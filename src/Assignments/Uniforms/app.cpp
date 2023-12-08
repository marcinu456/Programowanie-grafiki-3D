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
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
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

    // A vector containing the x,y,z vertex coordinates for the triangle.
   std::vector<GLfloat> vertices = {
   -0.5f, 0.0f, 0.0f,  0.1f,0.5f,0.1f,
   0.5f, 0.0f, 0.0f,   0.1f,0.5f,0.1f,
   0.0f, 0.5f, 0.0f,   0.1f,0.5f,0.1f,

   -0.5f, -0.5f, 0.0f, 0.5f,0.1f,0.1f,
   0.5f, -0.5f, 0.0f,  0.5f,0.1f,0.1f,
   -0.5f,  0.0f, 0.0f, 0.5f,0.1f,0.1f,

   0.5f, -0.5f, 0.0f,  0.1f,0.1f,0.5f,
   -0.5f, 0.0f, 0.0f,  0.1f,0.1f,0.5f,
   0.5f,  0.0f, 0.0f,  0.1f,0.1f,0.5f};

    // A vector containing the x,y,z vertex coordinates for the triangle.
   // std::vector<GLfloat> vertices = {
   //     -0.5f, 0.0f, 0.0f,  0.81f,0.1f,0.1f,
   //     0.5f, 0.0f, 0.0f,   0.81f,0.1f,0.1f,
   //     0.0f, 0.5f, 0.0f,   0.81f,0.1f,0.1f,
   //     -0.5f, 0.0f, 0.0f,  0.31f,0.1f,0.251f,
   //     -0.5f, -0.5f, 0.0f, 0.31f,0.1f,0.251f,
   //     0.5f, -0.5f, 0.0f,  0.31f,0.1f,0.251f,
   //     0.5f,  0.0f, 0.0f,  0.31f,0.1f,0.251f};
//
   // std::vector<GLushort> indices = {0,1,2,3,4,5,6};

    std::vector<GLushort> indices = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
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

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));
    

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBindVertexArray(0);
    //end of vao "recording"

    Uniform();
    MovingHouse();

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto[w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 9);
    glBindVertexArray(0);
    
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

void SimpleShapeApplication::MovingHouse()
{
    //Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;

    float theta = 1.0*glm::pi<float>()/6.0f;
    auto cs = std::cos(theta);
    auto ss = std::sin(theta);  
    glm::mat2 rot{cs, -ss, ss, cs};
    glm::vec2 trans{0.0,  -0.25};
    glm::vec2 scale{1.5, 0.5};

    glGenBuffers(1, &v_buffer_handle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, v_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, 12 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 4 * sizeof(GLfloat), glm::value_ptr(rot[0]));
    glBufferSubData(GL_UNIFORM_BUFFER, 16, 4 * sizeof(GLfloat), glm::value_ptr(rot[1]));
    glBufferSubData(GL_UNIFORM_BUFFER, 32, 2 * sizeof(GLfloat), glm::value_ptr(trans));
    glBufferSubData(GL_UNIFORM_BUFFER, 40, 2 * sizeof(GLfloat), glm::value_ptr(scale));
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
}