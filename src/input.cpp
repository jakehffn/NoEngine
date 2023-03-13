#include "input.hpp"

Input::Input() :
    active{ std::unordered_set<SDL_Keycode>() },
    added{ std::unordered_set<SDL_Keycode>() },
    removed{ std::unordered_set<SDL_Keycode>() },
    mouse_x{ 0 }, mouse_y{ 0 }, quit{ false } {}

void Input::update() {

    // Set mouse_x and mouse_y to the mouse positions in pixels
    SDL_GetMouseState(&this->mouse_x, &this->mouse_y);

    this->added.clear();
    this->removed.clear();

    // Handle events on queue
    while (SDL_PollEvent( &e ) != 0) {
        
        if (e.type == SDL_QUIT) {
            this->quit = true;
        } 

        SDL_Keycode current_key = e.key.keysym.sym;
        
        if (e.type == SDL_KEYDOWN) {

            if (!this->isActive(current_key)){

                this->active.insert(current_key);
                this->added.insert(current_key);
            }

        } else if (e.type == SDL_KEYUP) {

            this->active.erase(current_key);
            this->removed.insert(current_key);
        }
    }
}

bool Input::isActive(SDL_Keycode keycode) {
    return (this->active.count(keycode) > 0);
}

bool Input::isAdded(SDL_Keycode keycode) {
    return (this->added.count(keycode) > 0);
}

bool Input::isRemoved(SDL_Keycode keycode) {
    return (this->removed.count(keycode) > 0);
}

int Input::getMouseX() {
    return this->mouse_x;
}

int Input::getMouseY() {
    return this->mouse_y;
}

bool Input::isQuit() {
    return (this->isActive(SDLK_ESCAPE) || quit);
}