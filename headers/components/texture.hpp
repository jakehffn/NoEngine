#pragma once

#include <string>

#include <glm\glm.hpp>

// GLEW must come before OpenGL
#include <GL\glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "animation_structs.hpp"

struct Texture {

    std::string sprite_sheet_name;

    FrameData* frame_data;
};