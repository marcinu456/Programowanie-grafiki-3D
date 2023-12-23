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
        0.5f, 0.0f, 0.0f,   0.1f,0.5f,0.1f,
        0.0f, 0.5f, 0.0f,   0.1f,0.5f,0.1f,
        -0.5f, 0.0f, 0.0f,  0.1f,0.5f,0.1f,
        -0.5f, -0.5f, 0.0f, 0.5f,0.1f,0.1f,
        0.5f, -0.5f, 0.0f,  0.1f,0.1f,0.5f};

    
    std::vector<GLushort> indices = {0,1,2,0,2,3,3,4,0};
    
    
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
    PVM();

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
    //glDrawArrays(GL_TRIANGLES, 0, 9);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
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

void SimpleShapeApplication::PVM()
{
    int width = frame_buffer_size().first;
    int height = frame_buffer_size().second;
    
    //PVM
    glm::mat4 Projection = glm::perspective(
        glm::radians(45.0f), // field of view in degrees
        (float)width / (float)height, // aspect ratio
        0.1f, // near plane
        100.0f); // far plane
    
    glm::mat4 View = glm::lookAt(
        glm::vec3(3.0f, 2.0f, 4.0f), // Camera is at (3,2,4), in World Space
        glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
        glm::vec3(0.0f, 1.0f, 0.0f));  // Head is up (set to 0,-1,0 to look upside-down)
        
    glm::mat4 Model = glm::mat4(.5f);

    glm::mat4 PVM = Projection * View * Model;


    //Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    
    //Generating PVM buffer 
    glGenBuffers(1, &v_buffer_handle);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, v_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);    
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(PVM));
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
}