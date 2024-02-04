#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "Material.h"

namespace xe
{
    class ColorMaterial : public Material {
    public:

        ColorMaterial(const glm::vec4 color, GLuint texture, GLuint texture_unit) : color_(color), texture_(texture), texture_unit_(texture_unit) {}

        ColorMaterial(const glm::vec4 color, GLuint texture) : ColorMaterial(color, texture,0) {}
        
        ColorMaterial(const glm::vec4 color) : ColorMaterial(color, 0) {}

        void bind() override;

        static void init();

        static GLuint program() { return shader_; }

        void set_texture(const GLuint &texture) { texture_ = texture; }
        void set_texture_unit(const GLuint &texture_unit) { texture_unit_ = texture_unit; }
        
        GLuint get_texture() { return texture_; }
        GLuint get_texture_unit() { return texture_unit_; }

    private:
        static GLuint shader_;
        static GLuint color_uniform_buffer_;

        glm::vec4 color_;

        static GLint uniform_map_Kd_location_;
        GLuint texture_;
        GLuint texture_unit_;
    };
}
