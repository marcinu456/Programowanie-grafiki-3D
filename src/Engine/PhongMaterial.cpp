#include "PhongMaterial.h"


namespace xe
{
    GLuint PhongMaterial::shader_ = 0u;
    GLuint PhongMaterial::material_uniform_buffer_ = 0u;
    GLint PhongMaterial::uniform_ambient_location_ = 0;
    GLint  PhongMaterial::uniform_map_Kd_location_ = 0;
    
    void PhongMaterial::bind() {

        if(texture_ > 0) {
            glUniform1i(uniform_map_Kd_location_, texture_unit_);
            glActiveTexture(GL_TEXTURE0 + texture_unit_);
            glBindTexture(GL_TEXTURE_2D, texture_);
        }
        bool bUse_map_Kd = texture_ > 0;

        
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer_);

        glBindBuffer(GL_UNIFORM_BUFFER, material_uniform_buffer_);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), &Ks_[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &Ka_[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(GLint), &bUse_map_Kd);
        glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + sizeof(float), sizeof(GLfloat), &Ns);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
        
        
    }


    void PhongMaterial::init() {
        auto program = xe::utils::create_program(
                {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/phong_vs.glsl"},
                 {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/phong_fs.glsl"}});
        if (!program) {
            std::cerr << "Invalid program" << std::endl;
            exit(-1);
        }

        shader_ = program;

        glGenBuffers(1, &material_uniform_buffer_);
        glBindBuffer(GL_UNIFORM_BUFFER, material_uniform_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4) + 2 * sizeof(float), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);

        uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
        if (uniform_map_Kd_location_ == -1) {
            std::cerr << "Cannot get uniform map_Kd location" << std::endl;
        }
        uniform_ambient_location_ = glGetUniformLocation(shader_, "ambient_light");
#if __APPLE__
        auto u_modifiers_index = glGetUniformBlockIndex(program, "Color");
        if (u_modifiers_index == -1) {
            std::cerr << "Cannot find Color uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_modifiers_index, 0);
        }
#endif

#if __APPLE__
        auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
        if (u_transformations_index == -1) {
            std::cerr << "Cannot find Transformations uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_transformations_index, 1);
        }
#endif


    }
}