// From opengl-tutorials
#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GL/glu.h>

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path, std::vector<std::string>& logs);
