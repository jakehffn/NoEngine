#pragma once

#include <entt/entt.hpp>

#include "System.h"

#include <unordered_set>
#include <cassert>
#include <SDL.h>

#include "Spacial.h"
#include "Input.h"
#include "SpriteState.h"

#include "consts.h"

class InputSystem : public System {
public:
    InputSystem(entt::registry& registry);

    void update(entt::registry& registry, float deltaTime);
    bool isQuit();

    void systemState() override;

private:
    void collectInputs();
    void updateSpacial(Spacial& spacial, Input input, float deltaTime);

    void setToggle(SDL_Keycode toggle);
    void setAbridge(SDL_Keycode abridge);

    SpriteStatePair getEntityState(SpriteStatePair prevState);

    bool isKeyDown(SDL_Keycode key);
    int getMouseX();
    int getMouseY();

    std::unordered_set<SDL_Keycode> keyInputs;
    int mouseX;
    int mouseY;

    std::unordered_set<SDL_Keycode> toggleInputs; 
    std::unordered_set<SDL_Keycode> abridgedInputs;

    SDL_Event e;
    bool quit;
};