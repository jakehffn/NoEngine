#include "Game.h"
#include <iostream>

Game::~Game() {
    // Deallocate Program
    glDeleteProgram(programID);
}

void Game::init(SDL_Window* window) {

    this->window = window;

    this->clock = new Clock();
    this->input = new Input();

    this->cameraController = new UserCameraController(this->window, this->clock, this->input);

    this->scene = new Scene(this->window, this->clock, this->input, this->cameraController);

    // Enable text input
    SDL_StartTextInput();
}

void Game::run() {

    ShaderProgram* shaderProgram = new BasicShader();
    int shaderID = scene->addShaderProgram(shaderProgram);

    char spritePath[] = "./src/assets/sprites/BagHead.png";
    int spriteID = scene->addSprite(spritePath);

    scene->addInstance(spriteID, shaderID, glm::vec3(200, 200, 0), glm::vec3(900, 900, 1));

    // While application is running
    while(!input->quitProgram() && !input->isKeyDown(SDLK_ESCAPE)) {

        clock->tick();
        printf("FPS: %f\r", this->clock->getAverageFPS());

        // std::cout << clock->getFPS();
        // clock->getFPS();
        
        scene->render();

        // Update screen
        SDL_GL_SwapWindow(window);
    }

    std::cout << "\nOpenGLError Code: "<< glGetError() << "\n";

    SDL_StopTextInput();
}