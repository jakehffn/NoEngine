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

    char spritePath[] = "./src/assets/sprites/BagHead.png";
    int spriteID = scene->addSprite(spritePath);

    scene->addGameObject(spriteID, glm::vec3(200, 200, 0));

    // While application is running
    while(!input->quitProgram() && !input->isKeyDown(SDLK_ESCAPE)) {

        clock->tick();
        printf("FPS: %f\r", this->clock->getAverageFPS());

    
        
        scene->render();

        // Update screen
        SDL_GL_SwapWindow(window);
    }

    std::cout << "\nOpenGLError Code: "<< glGetError() << "\n";

    SDL_StopTextInput();
}