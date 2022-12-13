#include "InputManager.h"

InputManager::InputManager() :
    active{ std::unordered_set<SDL_Keycode>() },
    added{ std::unordered_set<SDL_Keycode>() },
    removed{ std::unordered_set<SDL_Keycode>() },
    mouseX{ 0 }, mouseY{ 0 }, quit{ false } {}

void InputManager::update() {

    // Set mouseX and mouseY to the mouse positions in pixels
    SDL_GetMouseState(&this->mouseX, &this->mouseY);

    this->added.clear();
    this->removed.clear();

    // Handle events on queue
    while (SDL_PollEvent( &e ) != 0) {
        
        if (e.type == SDL_QUIT) {
            this->quit = true;
        } 

        SDL_Keycode currentKey = e.key.keysym.sym;
        
        if (e.type == SDL_KEYDOWN) {

            this->active.insert(currentKey);
            this->added.insert(currentKey);

        } else if (e.type == SDL_KEYUP) {

            this->active.erase(currentKey);
            this->removed.insert(currentKey);
        }
    }
}

bool InputManager::isActive(SDL_Keycode keycode) {
    return (this->active.count(keycode) > 0);
}

bool InputManager::isAdded(SDL_Keycode keycode) {
    return (this->added.count(keycode) > 0);
}

bool InputManager::isRemoved(SDL_Keycode keycode) {
    return (this->removed.count(keycode) > 0);
}

int InputManager::getMouseX() {
    return this->mouseX;
}

int InputManager::getMouseY() {
    return this->mouseY;
}

bool InputManager::isQuit() {
    return (this->isActive(SDLK_ESCAPE) || quit);
}