#include "Game.h"

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

    char spritePath[] = "./src/assets/sprites/wikicat.png";
    int spriteID = scene->addSprite(spritePath);

    scene->addInstance(spriteID, shaderID, glm::vec3(0, 0, 0), glm::vec3(10, 20, 10));

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

    SDL_StopTextInput();
}