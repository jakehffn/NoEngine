#include "input.hpp"

Input::Input() {}

void Input::update() {
    DEBUG_TIMER(_, "Input::update");
    // Set mouse_x and mouse_y to the mouse positions in pixels
    {
        DEBUG_TIMER(prep_timer, "Prep");
        SDL_GetMouseState(&this->mouse_x, &this->mouse_y);

        this->added.clear();
        this->removed.clear();

        memset(this->mouse_added.data(), false, this->mouse_added.size());
        memset(this->mouse_removed.data(), false, this->mouse_removed.size());
    }
    {
        DEBUG_TIMER(event_handle_timer, "Event Handling");
        size_t num_events{0};
        SDL_Event event;
        SDL_Keycode current_key;
        Uint8 current_mouse_button;
        // Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            #ifndef NDEBUG
                ImGui_ImplSDL2_ProcessEvent(&event);
            #endif
            
            switch (event.type) {
                case SDL_MOUSEMOTION:
                    break;
                case SDL_KEYDOWN:
                    current_key = event.key.keysym.sym;
                    if (!this->isActive(current_key)){
                        this->active.insert(current_key);
                        this->added.insert(current_key);
                    }
                    break;
                case SDL_KEYUP:
                    current_key = event.key.keysym.sym;
                    this->active.erase(current_key);
                    this->removed.insert(current_key);
                    break;
                case SDL_MOUSEBUTTONDOWN: 
                    current_mouse_button = event.button.button;
                    if (!this->isMouseActive(current_mouse_button)){
                        this->mouse_active[current_mouse_button] = true;
                        this->mouse_added[current_mouse_button] = true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    current_mouse_button = event.button.button;
                    this->mouse_active[current_mouse_button] = false;
                    this->mouse_removed[current_mouse_button] = true;
                    break;
                case SDL_QUIT:
                    this->quit = true;
                    break;
                default:
                    break;
            }
        }
    }
}

bool Input::isActive(SDL_Keycode keycode) {
    return this->active.contains(keycode);
}

bool Input::isAdded(SDL_Keycode keycode) {
    return this->added.contains(keycode);
}

bool Input::isRemoved(SDL_Keycode keycode) {
    return this->removed.contains(keycode);
}

int Input::getMouseX() {
    return this->mouse_x;
}

int Input::getMouseY() {
    return this->mouse_y;
}

bool Input::isMouseActive(Uint8 mouse_button) {
    return this->mouse_active[mouse_button];
}

bool Input::isMouseAdded(Uint8 mouse_button) {
    return this->mouse_added[mouse_button];
}

bool Input::isMouseRemoved(Uint8 mouse_button) {
    return this->mouse_removed[mouse_button];
}

bool Input::isQuit() {
    return (this->isActive(SDLK_ESCAPE) || quit);
}

bool Input::interactionEnabled() {
    return this->interaction_enabled;
}

void Input::enableInteraction() {
    this->interaction_enabled = true;
}

void Input::disableInteraction() {
    this->interaction_enabled = false;
}