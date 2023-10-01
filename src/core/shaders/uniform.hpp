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

struct UniformData {
    size_t size;
    size_t alignment;
    bool is_array_type;
};

const UniformData UNIFORM_TYPE_INFO[] = {
    UniformData(4, 4, false),
    UniformData(4, 16, true),
    UniformData(4, 4, false),
    UniformData(4, 16, true),
    UniformData(8, 8, false),
    UniformData(8, 16, true),
    UniformData(12, 16, false),
    UniformData(12, 16, true),
    UniformData(16, 16, false),
    UniformData(16, 16, true),
    UniformData(64, 16, false),
    UniformData(64, 16, true),
    UniformData(4, 4, false),
    UniformData(4, 4, false),
};

struct Uniform {
    UniformDataType type;
    std::string name;
    size_t array_size;
    GLuint location;
    size_t buffer_offset;
};