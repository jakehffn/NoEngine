#include "InputSystem.h"
#include <iostream>

InputSystem::InputSystem(entt::registry& registry) :
    keyInputs{ std::unordered_set<SDL_Keycode>() },
    toggleInputs{ std::unordered_set<SDL_Keycode>() },
    abridgedInputs{ std::unordered_set<SDL_Keycode>() },
    mouseX{ 0 }, mouseY{ 0 }, quit{ false } {}

void InputSystem::update(entt::registry& registry, float deltaTime) {

    this->collectInputs();

    auto entities = registry.view<Input, Spacial, SpriteState>();

    for (auto entity : entities) {

        auto [input, spacial, spriteState] = entities.get(entity);

        SpriteStatePair newState = this->getEntityState(spriteState.state);

        this->updateSpacial(spacial, input, deltaTime, newState);

        if (newState != spriteState.state) {

            registry.patch<SpriteState>(entity, [newState](auto &state) { 

                state.state = newState; 
            });
        }
    }

    auto spriteStateEntities = registry.view<SpriteState>();

    for (auto entity : spriteStateEntities) {

        auto& spriteState = spriteStateEntities.get<SpriteState>(entity);

        
    }
}

void InputSystem::updateSpacial(Spacial& spacial, Input input, float deltaTime, SpriteStatePair state) {

    glm::vec3 forward(0, 1, 0);
    glm::vec3 right(1, 0, 0);

    glm::vec3 direction(0, 0, 0);

    if (input.bidirectional) {
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
    } else if (std::get<entity_c::ENTITY_STATE>(state) == entity_c::MOVING) {

        switch(std::get<entity_c::ENTITY_DIR>(state)) {
            case entity_c::UP:
                direction -= forward;
                break;
            case entity_c::DOWN:
                direction += forward;
                break;
            case entity_c::LEFT:
                direction -= right;
                break;
            case entity_c::RIGHT:
                direction += right;
                break;
        }
    }

    float dirLength = glm::length(direction);

    if (dirLength != 0.0f) {
        spacial.pos += (direction * (1/dirLength) * input.pps * deltaTime);
    }
}

SpriteStatePair InputSystem::getEntityState(SpriteStatePair prevStatePair) {

    bool up = isKeyDown(SDLK_w);
    bool down = isKeyDown(SDLK_s);
    bool left = isKeyDown(SDLK_a);
    bool right = isKeyDown(SDLK_d);

    bool vMovement = (up != down);
    bool hMovement = (left != right);

    entity_c::ENTITY_STATE returnState = (!(vMovement || hMovement) ? entity_c::IDLE : entity_c::MOVING);

    entity_c::ENTITY_STATE prevState = std::get<entity_c::ENTITY_STATE>(prevStatePair);
    entity_c::ENTITY_DIR prevDir = std::get<entity_c::ENTITY_DIR>(prevStatePair);

    entity_c::ENTITY_DIR returnDir = prevDir;

    entity_c::ENTITY_DIR vDir = entity_c::NONE;
    entity_c::ENTITY_DIR hDir = entity_c::NONE;

    if (vMovement) {
        if (up) {
            vDir = entity_c::UP;
        } else {
            vDir = entity_c::DOWN;
        }
    }

    if (hMovement) {
        if (left) {
            hDir = entity_c::LEFT;
        } else {
            hDir = entity_c::RIGHT;
        }
    }

    if (vDir != entity_c::NONE) {
        returnDir = vDir;
    } else if (hDir != entity_c::NONE) {
        returnDir = hDir;
    }

    if (prevState == entity_c::MOVING && returnState == entity_c::MOVING) {

        if ((vDir == prevDir || hDir == prevDir)) {
            returnDir = prevDir;
        } 
    } 

    return std::make_pair(returnState, returnDir);
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