#include "Input.h"

Input::Input() :
    keyInputs{ std::unordered_set<SDL_Keycode>() },
    toggleInputs{ std::unordered_set<SDL_Keycode>() },
    abridgedInputs{ std::unordered_set<SDL_Keycode>() },
    mouseX{ 0 }, mouseY{ 0 }, quit{ false }{}

void Input::update() {

    // Set mouseX and mouseY to the mouse positions in pixels
    SDL_GetMouseState(&this->mouseX, &this->mouseY);

    // For every abridged input, if it is in keyInputs, remove it
    //  This will cause any adbridged input to only be active for one frame
    for (auto abridgedInput : abridgedInputs) {
        keyInputs.erase(abridgedInput);
    }

    // Handle events on queue
    while (SDL_PollEvent( &e ) != 0) {
        
        // If windows native quit action, flag quit
        if (e.type == SDL_QUIT) {

            this->quit = true;
        } 
        
        // If it is a key event...
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            
            int isKeydown = (int)(e.type == SDL_KEYDOWN);

            SDL_Keycode currEventKey = e.key.keysym.sym;

            // On keydown only, toggle keys' states are changed
            if (toggleInputs.count(currEventKey) == 1 && isKeydown) {

                if (keyInputs.count(currEventKey) == 1) {

                    keyInputs.erase(currEventKey);
                } else {
                    keyInputs.insert(currEventKey);
                }

            } else {
                
                // For both normal and abridged keys, on keydown, insert into active inputs
                //  Note: there is an extraneous erase() for abridged keys as they will
                //      always be removed from the active inputs at the beginning of 
                //      an update
                if(isKeydown) {
                    keyInputs.insert(currEventKey);
                } else {
                    keyInputs.erase(currEventKey);
                }
            }
        } 
    }
}

void Input::setToggle(SDL_Keycode key) {

    assert(this->abridgedInputs.count(key) == 0);

    this->toggleInputs.insert(key);
}

void Input::setAbridge(SDL_Keycode key) {

    assert(this->toggleInputs.count(key) == 0);

    this->abridgedInputs.insert(key);
}

bool Input::isKeyDown(SDL_Keycode key) {
    return (this->keyInputs.count(key));
} 

int Input::getMouseX() {
    return this->mouseX;
}

int Input::getMouseY() {
    return this->mouseY;
}

bool Input::quitProgram() {
    return quit;
}