#pragma once

#include <memory>
#include <functional>

#include <entt/entt.hpp>

#include "spacial.hpp"
#include "text.hpp"
#include "dialog_child.hpp"
#include "destroy.hpp"
#include "gui_element.hpp"
#include "children.hpp"

struct DialogStep {
    entt::entity entity;
    virtual ~DialogStep() {};
    DialogStep* next_step{NULL};
    virtual void begin(entt::registry& registry, entt::entity parent) = 0;
    virtual DialogStep* next(entt::registry& registry, entt::entity parent, float dt) = 0;
};

struct DialogText : public DialogStep {
    Text text;
    float text_speed{50.0};
    float timer{0};
    size_t current_char{0};

    DialogText(Text text) {
        this->text = text;
    }

    void begin(entt::registry& registry, entt::entity parent) {
        this->entity = registry.create();

        auto spacial = registry.get<Spacial>(parent);

        registry.emplace<Spacial>(this->entity, spacial);
        registry.emplace<DialogChild>(this->entity);
        registry.emplace<Text>(this->entity, this->text);

        auto& children = registry.get<Children>(parent);
        children.children.push_back(this->entity);
    }

    DialogStep* next(entt::registry& registry, entt::entity parent, float dt) {
        this->timer -= dt;
        if (this->timer <= 0) {
            this->timer = this->text_speed;
            this->current_char++;
        }

        if (this->current_char >= 10) {
            this->next_step->begin(registry, parent);
            return this->next_step;
        }
        return this;
    }
};

struct DialogEnd : public DialogStep {
    void begin(entt::registry& registry, entt::entity parent) {
        registry.emplace<Destroy>(parent);
        auto& children = registry.get<Children>(parent);
        for (auto child : children.children) {
            registry.emplace<Destroy>(child);
        }
    }

    DialogStep* next(entt::registry& registry, entt::entity parent, float dt) {
        return NULL;
    }
};