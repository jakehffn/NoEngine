#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

// GLEW must come before OpenGL
#include <gl\glew.h>
#include <SDL.h>

#include "consts.hpp"
#include "game.hpp"

#include "component_grid.hpp"

bool init();
void close();

SDL_Window* window = NULL;
SDL_GLContext context;

// Initializes SDL, GLEW, then OpenGL
bool init() {
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;

	} else {
		// Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		window = SDL_CreateWindow("Untitled RPG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			render_c::SCREEN_WIDTH, render_c::SCREEN_HEIGHT, 
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS
		);

		if(window == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;

		} else {
			context = SDL_GL_CreateContext(window);

			if(context == NULL) {
				printf("SDL: OpenGL context could not be created!\nSDL Error: %s\n", SDL_GetError());
				success = false;
			} else {
				glewExperimental = GL_TRUE; 
				GLenum glew_error = glewInit();
				if( glew_error != GLEW_OK ) {
					printf("GLEW: Error initializing! %s\n", glewGetErrorString(glew_error));
				}

				// Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
					printf("SDL: Warning: Unable to set VSync!\nSDL Error: %s\n", SDL_GetError());
				}
			}
		}
	}

	return success;
}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* user_param ) {

	std::string source_string;
	std::string type_string;
	std::string severity_string;

	switch(source) {
		case GL_DEBUG_SOURCE_API:
			source_string = "GL_DEBUG_SOURCE_API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			source_string = "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			source_string = "GL_DEBUG_SOURCE_SHADER_COMPILER";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			source_string = "GL_DEBUG_SOURCE_THIRD_PARTY";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			source_string = "GL_DEBUG_SOURCE_APPLICATION";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			source_string = "GL_DEBUG_SOURCE_OTHER";
			break;
		default:
			source_string = "OTHER";
	};

	switch(type) {
		case GL_DEBUG_TYPE_ERROR:
			type_string = "GL_DEBUG_TYPE_ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			type_string = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			type_string = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			type_string = "GL_DEBUG_TYPE_PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			type_string = "GL_DEBUG_TYPE_PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_MARKER:
			type_string = "GL_DEBUG_TYPE_MARKER";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			type_string = "GL_DEBUG_TYPE_PUSH_GROUP";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			type_string = "GL_DEBUG_TYPE_POP_GROUP";
			break;
		case GL_DEBUG_TYPE_OTHER:
			type_string = "GL_DEBUG_TYPE_OTHER";
			break;
		default:
			type_string = "OTHER";
	};

	switch(severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			severity_string = "GL_DEBUG_SEVERITY_HIGH";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			severity_string = "GL_DEBUG_SEVERITY_MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			severity_string = "GL_DEBUG_SEVERITY_LOW";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			severity_string = "GL_DEBUG_SEVERITY_NOTIFICATION";
			break;
		default:
			severity_string = "OTHER";
	};
	
  	fprintf( stderr, "GL CALLBACK: %s type = %s, source = %s, severity = %s, message = %s\n",
        ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
        type_string.c_str(), source_string.c_str(), severity_string.c_str(), message 
	);
}

void close() {

	// Destroy window	
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

// Parameters necessary for SDL_Main
int main(int argv, char** args) {

	if(!init()) {
		
		printf("Failed to initialize!\n");

	} else {

		printf("Initialized Successfully\n");

		// Enable debug output
		glEnable( GL_DEBUG_OUTPUT );
		glDebugMessageCallback( MessageCallback, 0 );
		// disable all messages with source `GL_DEBUG_SEVERITY_NOTIFICATION`
		glDebugMessageControl(
			GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DEBUG_SEVERITY_NOTIFICATION,
			0, NULL,
			GL_FALSE
		); 

		Game scene(window);
		scene.mainLoop();
	}

	close();

	return 0;
}