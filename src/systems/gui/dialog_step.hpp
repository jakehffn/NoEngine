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
#include "input.hpp"

struct DialogStep {
    entt::entity entity;
    virtual ~DialogStep() {};
    DialogStep* next_step{NULL};
    virtual void begin(entt::registry& registry, size_t& line_number, entt::entity parent, bool completed=false) = 0;
    virtual bool step(entt::registry& registry, size_t& line_number, entt::entity parent, float dt) = 0;
    virtual DialogStep* next() { return this->next_step; };
};

struct DialogText : public DialogStep {
    Text text;
    DialogStep* skip_to;
    float text_speed{50.0};
    float timer{0};
    size_t current_char{0};
    std::u32string full_text;

    DialogText(Text text) {
        this->text = text;
        this->full_text = this->text.text;
        this->text.text = U"";
    }

    void begin(entt::registry& registry, size_t& line_number, entt::entity parent, bool completed) {
        this->entity = registry.create();

        auto spacial = registry.get<Spacial>(parent);

        float offset_from_dialog_border{4};
        spacial.position += offset_from_dialog_border;
        float line_height{14};
        spacial.position.y += line_number * line_height;

        registry.emplace<Spacial>(this->entity, spacial);
        registry.emplace<DialogChild>(this->entity);
        if (completed) {
            this->text.text = this->full_text;
        }
        registry.emplace<Text>(this->entity, this->text);

        auto& children = registry.get<Children>(parent);
        children.children.push_back(this->entity);

        line_number++;
    }

    bool step(entt::registry& registry, size_t& line_number, entt::entity parent, float dt) {
        auto& input = registry.ctx().at<Input&>();
        if (this->current_char > 1 && input.isAdded(SDLK_SPACE)) {
            this->current_char = this->full_text.size();
            this->timer = 0;
            while (this->next_step != this->skip_to) {
                this->next_step->begin(registry, line_number, parent, true);
                this->next_step = this->next_step->next_step;
            }
        }
        this->timer -= dt;
        if (this->timer <= 0) {
            this->timer = this->text_speed;
            this->current_char++;
            registry.patch<Text>(this->entity, [this](auto& text) {
                text.text = this->full_text.substr(0, this->current_char);
            });
        }

        return this->current_char < this->full_text.size();
    }
};

struct DialogWaitForInput : public DialogStep {
    float blink_speed{600.0};
    float timer{0};
    bool on{false};

    void begin(entt::registry& registry, size_t& line_number, entt::entity parent, bool completed) {
        this->entity = registry.create();

        auto spacial = registry.get<Spacial>(parent);

        float offset_from_dialog_border{6};
        spacial.position += offset_from_dialog_border;

        float edge_space{18};

        auto& new_spacial = registry.emplace<Spacial>(this->entity, spacial);
        new_spacial.position += glm::vec3(
            4,
            spacial.dimensions.y - edge_space,
            0
        );
        registry.emplace<DialogChild>(this->entity);
        registry.emplace<Text>(this->entity);

        auto& children = registry.get<Children>(parent);
        children.children.push_back(this->entity);

        line_number = 0;
    }

    bool step(entt::registry& registry, size_t& line_number, entt::entity parent, float dt) {
        this->timer -= dt;
        if (this->timer <= 0) {
            this->timer = this->blink_speed;
            this->on = !this->on;
            registry.patch<Text>(this->entity, [this](auto& text) {
                if (this->on) {
                    text.text = U"▼";
                } else {
                    text.text = U"";
                }
            });
        }

        auto& input = registry.ctx().at<Input&>();
        bool interacted = input.isAdded(SDLK_SPACE);
        if (interacted) {
            auto& children = registry.get<Children>(parent);
            for (auto entity : children.children) {
                registry.emplace<Destroy>(entity);
            }
            children.children.clear();
        }
        return !interacted;
    }
};

struct DialogEnd : public DialogStep {
    void begin(entt::registry& registry, size_t& line_number, entt::entity parent, bool completed) {
        registry.emplace<Destroy>(parent);
        auto& children = registry.get<Children>(parent);
        for (auto child : children.children) {
            registry.emplace<Destroy>(child);
        }
        line_number = 0;
    }

    bool step(entt::registry& registry, size_t& line_number, entt::entity parent, float dt) {
        assert(!"Step() should never be called on DialogEnd");
        return true;
    }
};