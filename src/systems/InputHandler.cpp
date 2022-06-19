#include "InputHandler.h"

InputHandler::InputHandler(entt::registry& registry) :
    inputs{ std::unordered_set<SDL_Keycode>() },
    toggleInputs{ std::unordered_set<SDL_Keycode>() },
    abridgedInputs{ std::unordered_set<SDL_Keycode>() },
    mouseX{ 0 }, mouseY{ 0 }, quit{ false } {

        this->connect(sayHello, SDLK_SPACE, SDLK_LSHIFT, SDLK_RSHIFT);

        // Order matters
        this->connect(updateSpriteStates, SDLK_w, SDLK_a, SDLK_s, SDLK_d);
        this->connectKeyUp(updateSpriteStates, SDLK_w, SDLK_a, SDLK_s, SDLK_d);
        
        this->connect(updateVelocities, SDLK_w, SDLK_a, SDLK_s, SDLK_d);
        this->connectKeyUp(updateVelocities, SDLK_w, SDLK_a, SDLK_s, SDLK_d);

        this->connect(updateSpacials, SDLK_w, SDLK_a, SDLK_s, SDLK_d);
        this->connectKeyUp(updateSpacials, SDLK_w, SDLK_a, SDLK_s, SDLK_d);

        this->setAbridge(SDLK_SPACE);
    }

void InputHandler::update(entt::registry& registry) {

    // Update user input data
    this->collectInputs();
    this->useInputs(registry);
}

void InputHandler::sayHello(entt::registry& registry) {
    printf("\nHello nice\n");
}

void InputHandler::updateSpriteStates(entt::registry& registry) {

    auto entities = registry.view<Input, SpriteState>();
    
    for (auto entity : entities) {

        auto [input, spriteState] = entities.get(entity);

        SpriteStatePair newState = this->getSpriteState(spriteState.state);

        if (newState != spriteState.state) {

            registry.patch<SpriteState>(entity, [newState](auto &state) { 
                state.state = newState; 
            });
        }
    }

}

SpriteStatePair InputHandler::getSpriteState(SpriteStatePair prevStatePair) {

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

void InputHandler::updateVelocities(entt::registry& registry) {

    auto entities = registry.view<Input, Velocity, SpriteState>();

    double deltaTime = registry.ctx().at<Clock&>().getDeltaTime();

    for (auto entity : entities) {

        auto [input, velocity, spriteState] = entities.get(entity);

        registry.patch<Velocity>(entity, [this, &spriteState = spriteState](auto& velocity) {
            velocity = this->getVelocity(spriteState.state);;
        });
    }
}

Velocity InputHandler::getVelocity(SpriteStatePair state) {

    glm::vec2 velocity(0, 0);

    if (std::get<entity_c::ENTITY_STATE>(state) == entity_c::MOVING) {

        glm::vec2 forward(0, 1);
        glm::vec2 right(1, 0); 

        switch(std::get<entity_c::ENTITY_DIR>(state)) {
            case entity_c::UP:
                velocity -= forward;
                break;
            case entity_c::DOWN:
                velocity += forward;
                break;
            case entity_c::LEFT:
                velocity -= right;
                break;
            case entity_c::RIGHT:
                velocity += right;
                break;
        }
    }

    return Velocity{velocity};
}

void InputHandler::updateSpacials(entt::registry& registry) {

    auto entities = registry.view<Input, Spacial, Velocity>();
    double deltaTime = registry.ctx().at<Clock&>().getDeltaTime();

    for (auto entity : entities) {

        auto [input, spacial, velocity] = entities.get(entity);

        Spacial newSpacial = this->getSpacial(spacial, velocity, deltaTime, input.pps);

        if (newSpacial.pos != spacial.pos) {
            registry.patch<Spacial>(entity, [newSpacial](auto &spacial) { 
                spacial = newSpacial; 
            });
        }
    }
}

Spacial InputHandler::getSpacial(Spacial spacial, Velocity velocity, float deltaTime, float inputSpeed) {
    
    glm::vec3 posChange = glm::vec3(velocity.vel.x, velocity.vel.y, 0) * inputSpeed * deltaTime;
    
    spacial.pos += posChange;

    return spacial;
}

void InputHandler::collectInputs() {

    // Set mouseX and mouseY to the mouse positions in pixels
    SDL_GetMouseState(&this->mouseX, &this->mouseY);

    // For every abridged input, if it is in inputs, remove it
    //  This will cause any adbridged input to only be active for one frame
    for (auto abridgedInput : this->abridgedInputs) {
        this->inputs.erase(abridgedInput);
    }

    for (auto keyUp : this->keyUps) {
        this->keyUps.erase(keyUp);
    }

    // Handle events on queue
    while (SDL_PollEvent( &e ) != 0) {
        this->collectInput(e);
    }
}

void InputHandler::collectInput(SDL_Event e) {
    
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

            if (inputs.count(currEventKey) == 1) {

                inputs.erase(currEventKey);
            } else {
                inputs.insert(currEventKey);
            }

        } else {
            
            // For both normal and abridged keys, on keydown, insert into active inputs
            //  Note: there is an extraneous erase() for abridged keys as they will
            //      always be removed from the active inputs at the beginning of 
            //      an update
            if(isKeydown) {
                inputs.insert(currEventKey);
            } else {

                inputs.erase(currEventKey);
                keyUps.insert(currEventKey);
            }
        }
    }
}

void InputHandler::useInputs(entt::registry& registry) {

    std::vector<void (InputHandler::*) (entt::registry&)> funcs;

    for (auto input : this->inputs) {

        for (auto func : this->inputEvents[input]) {

            // Check if function is already in list to avoid running a function 
            //  multiple times in a frame
            if (std::find(funcs.begin(), funcs.end(), func) == funcs.end()) {
                funcs.push_back(func);
            }
        }
    }

    for (auto keyUp : this->keyUps) {

        for (auto func : this->keyUpEvents[keyUp]) {

            // Check if function is already in list to avoid running a function 
            //  multiple times in a frame
            if (std::find(funcs.begin(), funcs.end(), func) == funcs.end()) {
                funcs.push_back(func);
            }
        }
    }

    for (auto func : funcs) {
        (this->*func)(registry);
    }
}

void InputHandler::setToggle(SDL_Keycode key) {

    assert(this->abridgedInputs.count(key) == 0);

    this->toggleInputs.insert(key);
}

void InputHandler::setAbridge(SDL_Keycode key) {

    assert(this->toggleInputs.count(key) == 0);

    this->abridgedInputs.insert(key);
}

bool InputHandler::isKeyDown(SDL_Keycode key) {
    return (this->inputs.count(key));
} 

void InputHandler::connect(void (InputHandler::*func) (entt::registry&), SDL_Keycode key) {
    this->inputEvents[key].push_back(func);
}

template<typename... Keys>
void InputHandler::connect(void (InputHandler::*func) (entt::registry&), SDL_Keycode key, Keys... keys) {

    this->connect(func, key);
    this->connect(func, keys...);
}

void InputHandler::connectKeyUp(void (InputHandler::*func) (entt::registry&), SDL_Keycode key) {
    this->keyUpEvents[key].push_back(func);
}

template<typename... Keys>
void InputHandler::connectKeyUp(void (InputHandler::*func) (entt::registry&), SDL_Keycode key, Keys... keys) {

    this->connectKeyUp(func, key);
    this->connectKeyUp(func, keys...);
}

int InputHandler::getMouseX() {
    return this->mouseX;
}

int InputHandler::getMouseY() {
    return this->mouseY;
}

bool InputHandler::isQuit() {
    return (this->isKeyDown(SDLK_ESCAPE) || quit);
}

void InputHandler::systemState() {
    printf("No state currently\n");
}