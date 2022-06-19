#pragma once

#include <iostream>

#include <entt/entt.hpp>

#include "System.h"
#include "Clock.h"

#include <unordered_set>
#include <cassert>
#include <SDL.h>

#include "Spacial.h"
#include "Input.h"
#include "SpriteState.h"
#include "Velocity.h"

#include "consts.h"


class InputHandler : public System {
public:
    InputHandler(entt::registry& registry);

    void update(entt::registry& registry);
    bool isQuit();

    void systemState() override;

private:
    void collectInputs();
    void collectInput(SDL_Event e);
    void useInputs(entt::registry& registry);

    void setToggle(SDL_Keycode toggle);
    void setAbridge(SDL_Keycode abridge);

    bool isKeyDown(SDL_Keycode key);
    int getMouseX();
    int getMouseY();

    // Connect member function to key action
    template<typename... Keys>
    void connect(void (InputHandler::*) (entt::registry&), SDL_Keycode key, Keys... );
    void connect(void (InputHandler::*) (entt::registry&), SDL_Keycode key);

    template<typename... Keys>
    void connectKeyUp(void (InputHandler::*) (entt::registry&), SDL_Keycode key, Keys... );
    void connectKeyUp(void (InputHandler::*) (entt::registry&), SDL_Keycode key);

    std::unordered_set<SDL_Keycode> inputs; // List of active key inputs based on defined logic
    std::unordered_set<SDL_Keycode> keyUps; // List of keys released last frame
    int mouseX;
    int mouseY;

    std::unordered_set<SDL_Keycode> toggleInputs; 
    std::unordered_set<SDL_Keycode> abridgedInputs;

    std::unordered_map<SDL_Keycode, std::vector<void (InputHandler::*) (entt::registry&)>> inputEvents;
    std::unordered_map<SDL_Keycode, std::vector<void (InputHandler::*) (entt::registry&)>> keyUpEvents;

    SDL_Event e;
    bool quit;

    void sayHello(entt::registry& registry);

    void updateVelocities(entt::registry& registry);
    Velocity getVelocity(SpriteStatePair state);

    void updateSpacials(entt::registry& registry);
    Spacial getSpacial(Spacial spacial, Velocity velocity, float deltaTime, float inputSpeed);

    void updateSpriteStates(entt::registry& registry);
    SpriteStatePair getSpriteState(SpriteStatePair state);
};