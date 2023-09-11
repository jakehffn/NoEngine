#include <iostream>
#include <string>
#include <vector>

// GLEW must come before OpenGL
#include <gl\glew.h>
#include <SDL.h>

#include "consts.hpp"
#include "game.hpp"

#include "component_grid.hpp"

#ifndef NDEBUG
	#include "imgui/backends/imgui_impl_opengl3.h"
	#include "imgui/backends/imgui_impl_sdl2.h"
	#include "debug.hpp"
#endif


SDL_Window* window = NULL;
SDL_GLContext gl_context;
#ifndef NDEBUG
	DebugWindow debugWindow;
#endif

// Initializes SDL, GLEW, then OpenGL
bool initContext() {
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
		std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	} else {
		// Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		window = SDL_CreateWindow(
			"Untitled RPG", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			constant::SCREEN_WIDTH, 
			constant::SCREEN_HEIGHT, 
			SDL_WINDOW_OPENGL | 
			SDL_WINDOW_SHOWN
		);

		SDL_ShowCursor(SDL_DISABLE);

		if(window == NULL) {
			std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		} else {
			gl_context = SDL_GL_CreateContext(window);
			if(gl_context == NULL) {
				std::cerr << "SDL: OpenGL context could not be created!\nSDL Error: " << SDL_GetError() << "\n";
				success = false;
			} else {
				glewExperimental = GL_TRUE; 
				GLenum glew_error = glewInit();
				if(glew_error != GLEW_OK) {
					std::cerr << "GLEW: Error initializing! " << glewGetErrorString(glew_error) << "\n";
				}
				// Use Vsync
				if(SDL_GL_SetSwapInterval(1) < 0) {
					std::cerr << "SDL: Warning: Unable to set VSync!\nSDL Error: " << SDL_GetError() << "\n";
				}
			}
		}
	}

	return success;
}

void deinitContext() {
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#ifndef NDEBUG
	ImVec4 colorRgbToImVec4(float r, float g, float b, float a) {
		return ImVec4{r/255, g/255, b/255, a/255};
	}

	void initStyles() {
		ImGui::StyleColorsDark(NULL);
		auto& styles = ImGui::GetStyle();

		const auto main_bg_color = colorRgbToImVec4(45, 53, 59, 255);
		const auto main_text_color = colorRgbToImVec4(211, 198, 170, 255);
		const auto hover_color = colorRgbToImVec4(54, 63, 69, 255);
		const auto main_dark_color = colorRgbToImVec4(33, 39, 43, 255);
		const auto accent_one = colorRgbToImVec4(131, 192, 146, 255);

		styles.Colors[ImGuiCol_Text] = main_text_color;
		styles.Colors[ImGuiCol_TitleBg] = main_bg_color;
		styles.Colors[ImGuiCol_TitleBgActive] = hover_color;
		styles.Colors[ImGuiCol_TextSelectedBg] = hover_color;
		styles.Colors[ImGuiCol_FrameBg] = main_dark_color;
		styles.Colors[ImGuiCol_PopupBg] = main_bg_color;

		// ---- Window stuff ----
		styles.Colors[ImGuiCol_Border] = main_dark_color;
		styles.Colors[ImGuiCol_WindowBg] = main_bg_color;
		styles.WindowBorderSize = 1;
		styles.WindowPadding = {0, 0};

		// ---- Menu stuff ----
		styles.Colors[ImGuiCol_MenuBarBg] = main_bg_color;
		// This also changes the menu item's hover color
		styles.Colors[ImGuiCol_Header] = hover_color;
		styles.Colors[ImGuiCol_HeaderHovered] = hover_color;

		// ---- Button stuff ----
		styles.Colors[ImGuiCol_Button] = main_bg_color;
		styles.Colors[ImGuiCol_ButtonHovered] = hover_color;
		styles.Colors[ImGuiCol_ButtonActive] = main_bg_color;

		// ---- Slider stuff ----
		styles.Colors[ImGuiCol_SliderGrab] = main_bg_color;
		styles.Colors[ImGuiCol_SliderGrabActive] = main_bg_color; 

		styles.Colors[ImGuiCol_FrameBg] = main_dark_color;
		styles.Colors[ImGuiCol_FrameBgHovered] = main_dark_color;
		styles.Colors[ImGuiCol_FrameBgActive] = hover_color;
	}

	void initImGui() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		// io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init("#version 150");
		initStyles();
	}

	void deinitImGui() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void showDebugWindow() {
		debugWindow.show();
	}
#endif

void GLAPIENTRY
MessageCallback( 
	GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* user_param
) {

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
	
  	std::cerr << "GL CALLBACK:\ntype = " << type_string.c_str() << ", source = " << source_string.c_str() << 
		", severity = " << severity_string.c_str() << ", message = " << message << "\n" << 
		( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" );
}

// Parameters necessary for SDL_Main
int main(int argv, char** args) {

	if(!initContext()) {
		#ifndef NDEBUG
			std::cerr << "GL context failed to initialize!\n";
		#endif

	} else {
		#ifndef NDEBUG
			std::cout << "GL context initialized successfully\n";
			// Enable debug output
			glEnable( GL_DEBUG_OUTPUT );
			glDebugMessageCallback(MessageCallback, 0);
			// disable all messages with source `GL_DEBUG_SEVERITY_NOTIFICATION`
			glDebugMessageControl(
				GL_DONT_CARE,
				GL_DONT_CARE,
				GL_DEBUG_SEVERITY_NOTIFICATION,
				0, NULL,
				GL_FALSE
			); 

			initImGui();
		#endif

		Game game(window);
		#ifndef NDEBUG
			debugWindow = DebugWindow(&game);
			game.mainLoop(showDebugWindow);
		#else
			game.mainLoop(NULL);
		#endif

		#ifndef NDEBUG
			deinitImGui();
		#endif
		deinitContext();
	}

	return 0;
}