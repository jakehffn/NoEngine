#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

// GLEW must come before OpenGL
#include <gl\glew.h>
#include <SDL.h>

#include "consts.h"
#include "Game.h"

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
				GLenum glewError = glewInit();
				if( glewError != GLEW_OK ) {
					printf("GLEW: Error initializing! %s\n", glewGetErrorString(glewError));
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
                 const void* userParam ) {

	std::string sourceString;
	std::string typeString;
	std::string severityString;

	switch(source) {
		case GL_DEBUG_SOURCE_API:
			sourceString = "GL_DEBUG_SOURCE_API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			sourceString = "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			sourceString = "GL_DEBUG_SOURCE_SHADER_COMPILER";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			sourceString = "GL_DEBUG_SOURCE_THIRD_PARTY";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			sourceString = "GL_DEBUG_SOURCE_APPLICATION";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			sourceString = "GL_DEBUG_SOURCE_OTHER";
			break;
		default:
			sourceString = "OTHER";
	};

	switch(type) {
		case GL_DEBUG_TYPE_ERROR:
			typeString = "GL_DEBUG_TYPE_ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			typeString = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			typeString = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			typeString = "GL_DEBUG_TYPE_PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			typeString = "GL_DEBUG_TYPE_PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_MARKER:
			typeString = "GL_DEBUG_TYPE_MARKER";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			typeString = "GL_DEBUG_TYPE_PUSH_GROUP";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			typeString = "GL_DEBUG_TYPE_POP_GROUP";
			break;
		case GL_DEBUG_TYPE_OTHER:
			typeString = "GL_DEBUG_TYPE_OTHER";
			break;
		default:
			typeString = "OTHER";
	};

	switch(severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			severityString = "GL_DEBUG_SEVERITY_HIGH";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			severityString = "GL_DEBUG_SEVERITY_MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			severityString = "GL_DEBUG_SEVERITY_LOW";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			severityString = "GL_DEBUG_SEVERITY_NOTIFICATION";
			break;
		default:
			severityString = "OTHER";
	};
	
  	fprintf( stderr, "GL CALLBACK: %s type = %s, source = %s, severity = %s, message = %s\n",
        ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
        typeString.c_str(), sourceString.c_str(), severityString.c_str(), message 
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