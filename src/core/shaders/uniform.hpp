#pragma once

#include <string>

#include <GL\glew.h>

enum class UniformDataType {
	INT,
	INT_ARRAY,
	FLOAT,
	FLOAT_ARRAY,
	VEC_2,
	VEC_2_ARRAY,
	VEC_3,
	VEC_3_ARRAY,
	VEC_4,
	VEC_4_ARRAY,
	MAT_4X4,
	MAT_4X4_ARRAY,
	TEX_2D
};

inline UniformDataType convertGLType(GLuint type, bool is_array) {
    switch (type) {
        case GL_INT:
            return (is_array) ? UniformDataType::INT_ARRAY : UniformDataType::INT;
            break;
        case GL_FLOAT:
            return (is_array) ? UniformDataType::FLOAT_ARRAY : UniformDataType::FLOAT;
            break;
        case GL_FLOAT_VEC2:
            return (is_array) ? UniformDataType::VEC_2_ARRAY : UniformDataType::VEC_2;
            break;
        case GL_FLOAT_VEC3:
            return (is_array) ? UniformDataType::VEC_3_ARRAY : UniformDataType::VEC_3;
            break;
        case GL_FLOAT_VEC4:
            return (is_array) ? UniformDataType::VEC_4_ARRAY : UniformDataType::VEC_4;
            break;
        case GL_FLOAT_MAT4:
            return (is_array) ? UniformDataType::MAT_4X4_ARRAY : UniformDataType::MAT_4X4;
            break;
        case GL_SAMPLER_2D:
            return UniformDataType::TEX_2D;
            break;
    }
    assert(!"Unsupported shader uniform type");
    return UniformDataType::INT;
}

struct UniformData {
    size_t size;
    size_t alignment;
    bool is_array_type;
    const char* uniform_identifier;
};

const UniformData UNIFORM_TYPE_INFO[] = {
    UniformData(4, 4, false, "int"),
    UniformData(4, 16, true, "int"),
    UniformData(4, 4, false, "float"),
    UniformData(4, 16, true, "float"),
    UniformData(8, 8, false, "vec2"),
    UniformData(8, 16, true, "vec2"),
    UniformData(12, 16, false, "vec3"),
    UniformData(12, 16, true, "vec3"),
    UniformData(16, 16, false, "vec4"),
    UniformData(16, 16, true, "vec4"),
    UniformData(64, 16, false, "mat4"),
    UniformData(64, 16, true, "mat4"),
    UniformData(4, 4, false, "sampler2D"),
};

struct Uniform {
    UniformDataType type;
    std::string name;
    size_t array_size;
    GLuint location;
    size_t buffer_offset;
};