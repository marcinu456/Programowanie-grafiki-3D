#pragma once
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:

    Camera(){} //default constructor

    Camera(float fov = 0.8f, float aspect = 1, float near = 0.1f, float far = 100.0f, glm::mat4 model = glm::mat4(1.0f))
    {
        perspective(fov, aspect, near, far);
        set_model(model);
    }

    //Set View
    void look_at(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
        View = glm::lookAt(eye, center, up);
    }

    //Set Projection 
    void perspective(float fov, float aspect, float near, float far) {
        fov_ = fov;
        aspect_ = aspect;
        near_ = near;
        far_ = far;
    }

    //Set Model
    void set_model(glm::mat4 model) {
        Model = model;
    }

    void set_aspect(float aspect) {
        aspect_ = aspect;
    }

    glm::mat4 view() const { return View; }

    glm::mat4 projection() const { return glm::perspective(fov_, aspect_, near_, far_); }

    
    glm::mat4 GetPVM()
    {
        return  projection() * view() * Model;
    }
    
    void zoom(float y_offset) {
        auto x = fov_ / glm::pi<float>();
        auto y = inverse_logistic(x);
        y += y_offset;
        x = logistic(y);
        fov_ = x * glm::pi<float>();
    }

private:
    float fov_;
    float aspect_;
    float near_;
    float far_;

    glm::mat4 View;
    glm::mat4 Model;

    float logistic(float y) {
        return 1.0f / (1.0f + std::exp(-y));
    }

    float inverse_logistic(float x) {
        return std::log(x / (1.0f - x));
    }
};