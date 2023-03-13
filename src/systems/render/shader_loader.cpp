// From opengl-tutorials
#include "shader_loader.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <GL/glew.h>


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){

	// Create the shaders
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertex_shader_source;
	std::ifstream vertex_shader_stream(vertex_file_path, std::ios::in);

	if(vertex_shader_stream.is_open()){

		std::stringstream sstr;
		sstr << vertex_shader_stream.rdbuf();
		vertex_shader_source = sstr.str();
		vertex_shader_stream.close();

	}else{

		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string fragment_shader_source;
	std::ifstream fragment_shader_stream(fragment_file_path, std::ios::in);

	if(fragment_shader_stream.is_open()){

		std::stringstream sstr;
		sstr << fragment_shader_stream.rdbuf();
		fragment_shader_source = sstr.str();
		fragment_shader_stream.close();
	}

	GLint result = GL_FALSE;
	int info_log_length;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * vertex_source_pointer = vertex_shader_source.c_str();
	glShaderSource(vertex_shader_id, 1, &vertex_source_pointer , NULL);
	glCompileShader(vertex_shader_id);

	// Check Vertex Shader
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

	if ( info_log_length > 0 ){

		std::vector<char> vertex_shader_error_message(info_log_length+1);
		glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, &vertex_shader_error_message[0]);
		printf("%s\n", &vertex_shader_error_message[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * fragment_source_pointer = fragment_shader_source.c_str();
	glShaderSource(fragment_shader_id, 1, &fragment_source_pointer , NULL);
	glCompileShader(fragment_shader_id);

	// Check Fragment Shader
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

	if ( info_log_length > 0 ){

		std::vector<char> FragmentShaderErrorMessage(info_log_length+1);
		glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	// Check the program
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);

	if ( info_log_length > 0 ){

		std::vector<char> program_error_message(info_log_length+1);
		glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);
		printf("%s\n", &program_error_message[0]);
	}

	
	glDetachShader(program_id, vertex_shader_id);
	glDetachShader(program_id, fragment_shader_id);
	
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return program_id;
}

