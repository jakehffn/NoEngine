#include "InputSystem.h"

InputSystem::InputSystem(entt::registry& registry) :
    keyInputs{ std::unordered_set<SDL_Keycode>() },
    toggleInputs{ std::unordered_set<SDL_Keycode>() },
    abridgedInputs{ std::unordered_set<SDL_Keycode>() },
    mouseX{ 0 }, mouseY{ 0 }, quit{ false } {}

void InputSystem::update(entt::registry& registry, float deltaTime) {

    this->collectInputs();

    auto entities = registry.view<Input, Spacial>();

    for (auto entity : entities) {

        auto [input, spacial] = entities.get(entity);

        this->updateSpacial(spacial, input, deltaTime);
    }

    auto spriteStateEntities = registry.view<SpriteState>() | entities;

    for (auto entity : entities) {

        registry.patch<SpriteState>(entity, [this](auto &state) { 

            ENTITY_STATE newState = this->getEntityState(state.prevState);

            state.prevState = state.state;
            state.state = newState; 
            });
    }
}

void InputSystem::updateSpacial(Spacial& spacial, Input input, float deltaTime) {

    glm::vec3 forward(0, 1, 0);
    glm::vec3 right(1, 0, 0);

    glm::vec3 direction(0, 0, 0);

    // Move forward
    if (this->isKeyDown(SDLK_w)) {
        direction -= forward;
    }
    // Move backward
    if (this->isKeyDown(SDLK_s)) {
        direction += forward;
    }
    // Strafe left
    if (this->isKeyDown(SDLK_a)) {
        direction -= right;
    }
    // Strafe right
    if (this->isKeyDown(SDLK_d)) {
        direction += right;
    }

    float dirLength = glm::length(direction);

    if (dirLength != 0.0f) {
        spacial.pos += (direction * (1/dirLength) * input.pps * deltaTime);
    }
}

ENTITY_STATE InputSystem::getEntityState(ENTITY_STATE prevState) {

    bool up = isKeyDown(SDLK_w);
    bool down = isKeyDown(SDLK_s);
    bool left = isKeyDown(SDLK_a);
    bool right = isKeyDown(SDLK_d);

    bool verticalMovement = (up != down);
    bool horizontalMovement = (left != right);

    ENTITY_STATE verticalState = IDLE;
    ENTITY_STATE horizontalState = IDLE;

    ENTITY_STATE returnState = IDLE;

    if (verticalMovement) {
        if (up) {
            verticalState = MOVE_UP;
        } else {
            verticalState = MOVE_DOWN;
        }
    }

    if (horizontalMovement) {
        if (left) {
            horizontalState = MOVE_LEFT;
        } else {
            horizontalState = MOVE_RIGHT;
        }
    }

    if (verticalState != IDLE) {
        returnState = verticalState;
    } else if (horizontalState != IDLE) {
        returnState = horizontalState;
    }

    if ((horizontalState == prevState || verticalState == prevState) && prevState != IDLE) {
        returnState = prevState;
    }

    return returnState;

}

void InputSystem::collectInputs() {

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

void InputSystem::setToggle(SDL_Keycode key) {

    assert(this->abridgedInputs.count(key) == 0);

    this->toggleInputs.insert(key);
}

void InputSystem::setAbridge(SDL_Keycode key) {

    assert(this->toggleInputs.count(key) == 0);

    this->abridgedInputs.insert(key);
}

bool InputSystem::isKeyDown(SDL_Keycode key) {
    return (this->keyInputs.count(key));
} 

int InputSystem::getMouseX() {
    return this->mouseX;
}

int InputSystem::getMouseY() {
    return this->mouseY;
}

bool InputSystem::isQuit() {
    return (this->isKeyDown(SDLK_ESCAPE) || quit);
}

void InputSystem::systemState() {
    printf("No state currently\n");
}