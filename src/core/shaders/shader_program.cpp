#include "shader_program.hpp"

ShaderProgram::ShaderProgram(
    std::vector<Uniform> uniforms, 
    const char* vertex_source, 
    const char* fragment_source, 
    std::vector<std::string>& logs
) : logs{logs}, uniforms{uniforms}, vertex_source{vertex_source}, fragment_source{fragment_source} {
    this->id = LoadShaders(this->vertex_source, this->fragment_source, this->logs);
    this->initUniformBuffer();
    this->getUniformLocations();
}

ShaderProgram::~ShaderProgram() {
    free(this->uniform_buffer);
}

void ShaderProgram::use(){ 
    glUseProgram(this->id); 
    for (auto uniform : this->uniforms) {
        switch(uniform.type) {
            case UniformDataType::INT:
                glUniform1i(
                    uniform.location, 
                    *reinterpret_cast<GLint*>(this->uniform_buffer + uniform.buffer_offset)
                );
                break;
            case UniformDataType::INT_ARRAY:
                glUniform1iv(
                    uniform.location, 
                    uniform.array_size, 
                    reinterpret_cast<GLint*>(this->uniform_buffer + uniform.buffer_offset)
                );
                break;
            case UniformDataType::FLOAT:
                glUniform1f(
                    uniform.location, 
                    *reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset)
                );
                break;
            case UniformDataType::FLOAT_ARRAY:
                glUniform1fv(
                    uniform.location, 
                    uniform.array_size, 
                    reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset)
                );
                break;
            case UniformDataType::VEC_2:
                glUniform2f(
                    uniform.location, 
                    *reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset),
                    *reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset + 4)
                );
                break;
            case UniformDataType::VEC_2_ARRAY:
                glUniform2fv(
                    uniform.location, 
                    uniform.array_size, 
                    reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset)
                );
                break;
            case UniformDataType::VEC_3:
                glUniform3f(
                    uniform.location, 
                    *reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset),
                    *reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset + 4),
                    *reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset + 8)
                );
                break;
            case UniformDataType::VEC_3_ARRAY:
                glUniform3fv(
                    uniform.location, 
                    uniform.array_size, 
                    reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset)
                );
                break;
            case UniformDataType::VEC_4:
                glUniform4f(
                    uniform.location, 
                    *reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset),
                    *reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset + 4),
                    *reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset + 8),
                    *reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset + 12)
                );
                break;
            case UniformDataType::VEC_4_ARRAY:
                glUniform4fv(
                    uniform.location, 
                    uniform.array_size, 
                    reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset)
                );
                break;
            case UniformDataType::MAT_4X4:
                glUniformMatrix4fv(
                    uniform.location, 
                    1,
                    GL_FALSE,
                    reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset)
                );
                break;
            case UniformDataType::MAT_4X4_ARRAY:
                glUniformMatrix4fv(
                    uniform.location, 
                    uniform.array_size,
                    GL_FALSE,
                    reinterpret_cast<GLfloat*>(this->uniform_buffer + uniform.buffer_offset)
                );
                break;
            case UniformDataType::TEX_2D:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glBindTexture(
                    GL_TEXTURE_2D, 
                    *reinterpret_cast<GLuint*>(this->uniform_buffer + uniform.buffer_offset)
                );
                break;
            default:
                break;
        }
    }
};

void ShaderProgram::render(size_t num_verts, GLuint vao, GLuint dest_fbo){ 
    glBindFramebuffer(GL_FRAMEBUFFER, dest_fbo);
    this->use();
    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, num_verts); 
};

void ShaderProgram::recompile() {
    glDeleteShader(this->id);
    this->id = LoadShaders(this->vertex_source, this->fragment_source, this->logs);
    this->initUniformBuffer();
    this->getUniformLocations();
}

bool ShaderProgram::containsUniform(const char* name) {
    for (auto uniform : this->uniforms) {
        if (!strcmp(uniform.name.c_str(), name)) {
            return true;
        }
    }
    return false;
}

void ShaderProgram::setUniformBufferData(size_t offset, GLint uniform_data) {
    *reinterpret_cast<GLint*>(this->uniform_buffer + offset) = uniform_data;
}

void ShaderProgram::setUniformBufferDataV(size_t offset, size_t count, GLint* uniform_data) {
    memcpy(
        reinterpret_cast<GLint*>(this->uniform_buffer + offset), 
        uniform_data,
        count
    );
}

void ShaderProgram::setUniformBufferData(size_t offset, GLfloat uniform_data) {
    *reinterpret_cast<GLfloat*>(this->uniform_buffer + offset) = uniform_data;
}

void ShaderProgram::setUniformBufferDataV(size_t offset, size_t count, float* uniform_data) {
    memcpy(
        reinterpret_cast<GLfloat*>(this->uniform_buffer + offset), 
        uniform_data,
        count
    );
}

void ShaderProgram::setUniformBufferData(size_t offset, glm::vec2 uniform_data) {
    *reinterpret_cast<glm::vec2*>(this->uniform_buffer + offset) = uniform_data;
}

void ShaderProgram::setUniformBufferDataV(size_t offset, size_t count, glm::vec2* uniform_data) {
    memcpy(
        reinterpret_cast<glm::vec2*>(this->uniform_buffer + offset), 
        uniform_data,
        count
    );
}

void ShaderProgram::setUniformBufferData(size_t offset, glm::vec3 uniform_data) {
    *reinterpret_cast<glm::vec3*>(this->uniform_buffer + offset) = uniform_data;
}

void ShaderProgram::setUniformBufferDataV(size_t offset, size_t count, glm::vec3* uniform_data) {
    memcpy(
        reinterpret_cast<glm::vec3*>(this->uniform_buffer + offset), 
        uniform_data,
        count
    );
}
 
void ShaderProgram::setUniformBufferData(size_t offset, glm::vec4 uniform_data) {
    *reinterpret_cast<glm::vec4*>(this->uniform_buffer + offset) = uniform_data;
}

void ShaderProgram::setUniformBufferDataV(size_t offset, size_t count, glm::vec4* uniform_data) {
    memcpy(
        reinterpret_cast<glm::vec4*>(this->uniform_buffer + offset), 
        uniform_data,
        count
    );
}
 
void ShaderProgram::setUniformBufferData(size_t offset, glm::mat4 uniform_data) {
    *reinterpret_cast<glm::mat4*>(this->uniform_buffer + offset) = uniform_data;
}

void ShaderProgram::setUniformBufferDataV(size_t offset, size_t count, glm::mat4* uniform_data) {
    memcpy(
        reinterpret_cast<glm::mat4*>(this->uniform_buffer + offset), 
        uniform_data,
        count
    );
}

void ShaderProgram::setUniformBufferData(size_t offset, GLuint uniform_data) {
    *reinterpret_cast<GLuint*>(this->uniform_buffer + offset) = uniform_data;
}
 

void ShaderProgram::initUniformBuffer() {
    size_t buffer_size{0};

    for (size_t it{0}; it < this->uniforms.size(); it++) {
        auto& uniform{this->uniforms[it]};
        const auto type_info{UNIFORM_TYPE_INFO[static_cast<size_t>(uniform.type)]};
        
        uniform.buffer_offset = buffer_size;
        if (!type_info.is_array_type) {
            buffer_size += type_info.size;
        } else {
            buffer_size += type_info.size*uniform.array_size;
        }

        // Adjust for alignment
        if (it + 1 < this->uniforms.size()) {
            const auto& next_uniform{this->uniforms[it + 1]};
            const auto next_type_info{UNIFORM_TYPE_INFO[static_cast<size_t>(next_uniform.type)]};
            buffer_size += next_type_info.alignment - (buffer_size % next_type_info.alignment);
        }
    }

    this->buffer_size = buffer_size;
    this->uniform_buffer = static_cast<char*>(malloc(this->buffer_size));
}

void ShaderProgram::getUniformLocations() {
    for (auto& uniform : this->uniforms) {
        if (uniform.type != UniformDataType::TEX_2D) {
            uniform.location = glGetUniformLocation(this->id, uniform.name.c_str());
        }
    }
}