#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include "Material.h"

namespace xe
{
    class PhongMaterial : public Material {
    public:

        PhongMaterial(const glm::vec4 color, const glm::vec3 ambient, const glm::vec3 specular, float specularStrength) : Kd_(color), Ka_(ambient), Ks_(specular), Ns(specularStrength) {}

        PhongMaterial(const glm::vec4 color, const glm::vec3 ambient, const glm::vec3 specular, float specularStrength, GLuint texture, GLuint texture_unit) : Kd_(color), texture_(texture),
                                                                                    texture_unit_(texture_unit) {}

        PhongMaterial(const glm::vec4 color, GLuint texture, GLuint texture_unit) : Kd_(color), texture_(texture),
                                                                                    texture_unit_(texture_unit) {}

        PhongMaterial(const glm::vec4 color, GLuint texture) : PhongMaterial(color, texture, 0) {}

        PhongMaterial(const glm::vec4 color) : PhongMaterial(color, 0) {}

        void bind() override;

        static void init();

        static GLuint program() { return shader_; }

        void set_texture(const GLuint &texture) { texture_ = texture; }
        void set_texture_unit(const GLuint &texture_unit) { texture_unit_ = texture_unit; }
        
        GLuint get_texture() { return texture_; }
        GLuint get_texture_unit() { return texture_unit_; }

        static void set_ambient(const glm::vec3 ambient) {
            glUseProgram(program());
            glUniform3f(uniform_ambient_location_, ambient.x, ambient.y, ambient.z);
            glUseProgram(0);
        }

    private:
        static GLuint shader_;
        static GLuint material_uniform_buffer_;

        glm::vec4 color_;

        static GLint uniform_map_Kd_location_;
        GLuint texture_;
        GLuint texture_unit_;

        glm::vec4 Kd_;
        glm::vec3 Ka_;
        glm::vec3 Ks_;

        float Ns;

        static GLint uniform_ambient_location_;
    };
}
