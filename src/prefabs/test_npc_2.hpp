#pragma once

#include <cstdlib>

#include <entt/entt.hpp>

#include "camera_controller.hpp"
#include "collider.hpp"
#include "collidable.hpp"
#include "name.hpp"
#include "renderable.hpp"
#include "idle_animation.hpp"
#include "move_animation.hpp"
#include "player_control.hpp"
#include "velocity.hpp"
#include "state_machine.hpp"
#include "state_machine_builder.hpp"
#include "interactable.hpp"
#include "dialog.hpp"
#include "gui_element.hpp"
#include "map_loader.hpp"
#include "dialog_builder.hpp"

static void TestNpc2(entt::registry& registry, entt::entity entity) {
    registry.emplace<Collider>(entity);

    std::string sprite_sheet_name = "Npc";
    std::string sprite_sheet_id = "characters/kid";

    auto& sprite_sheet_atlas = registry.ctx().at<SpriteSheetAtlas&>();
    auto& sprite_sheet = sprite_sheet_atlas.initSpriteSheet(registry, sprite_sheet_id);
    auto& [default_animation_name, default_animation] = *(sprite_sheet.animations.begin());

    auto& animator = registry.emplace<Animator>(entity, &default_animation.frame_durations);
    registry.emplace<Animation>(entity, &animator, &default_animation);
    registry.emplace<Texture>(entity, sprite_sheet_name, default_animation.frames[0]);
    registry.emplace<IdleAnimation>(entity, registry, sprite_sheet_id, "Idle_Up", "Idle_Down", "Idle_Left", "Idle_Right");
    registry.emplace<MoveAnimation>(entity, registry, sprite_sheet_id, "Move_Up", "Move_Down", "Move_Left", "Move_Right");

    registry.emplace<Interactable>(entity, [](entt::registry& registry, entt::entity entity) {
        auto text_box_entity = registry.create();
        registry.emplace<Spacial>(
            text_box_entity, 
            glm::vec3(-200 + rand() % 200, rand() % 200 - 100, 1), 
            glm::vec2(200, 30)
        );
        registry.emplace<Dialog>(text_box_entity,
            DialogBuilder()
            .text("This is some test text. It should be long enought to extend past the end without formatting")
            .end()
        );
        registry.emplace<GuiElement>(text_box_entity);
        registry.emplace<Persistent>(text_box_entity);
        // auto& map_loader = registry.ctx().at<MapLoader&>();
        // map_loader.queueLoad("./assets/maps/Test/test.tmx");
    });

    registry.emplace<StateMachine>(entity, 
        StateMachineBuilder()
        .wait(2000)
        .then([](entt::registry& registry, entt::entity entity) {
            registry.emplace_or_replace<Velocity>(entity, glm::vec3(-80,0,0));
            registry.patch<Spacial>(entity, [](auto& spacial) {
                spacial.direction = LEFT;
            });
        })
        .wait(2000)
        .then([](entt::registry& registry, entt::entity entity) {
            registry.emplace_or_replace<Velocity>(entity, glm::vec3(80,0,0));
            registry.patch<Spacial>(entity, [](auto& spacial) {
                spacial.direction = RIGHT;
            });
        })
        .loop()
    );

    registry.emplace<Renderable>(entity);
}