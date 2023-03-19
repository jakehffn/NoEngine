#include "animation_system.hpp"

AnimationSystem::AnimationSystem(entt::registry& registry) : System(registry),
    idle_animation_observer{ entt::observer(registry, entt::collector.group<Texture, IdleAnimation>(entt::exclude<Velocity>)) }, 
    move_animation_observer{ entt::observer(registry, entt::collector.group<Texture, MoveAnimation, Velocity>()) } {
}

void AnimationSystem::update() {

    this->updateAnimators();
    this->updateTextures();
    
    this->updateIdleAnimations();
    this->updateMoveAnimations();
}

void AnimationSystem::updateAnimators() {

    Clock& clock = this->registry.ctx().at<Clock&>();

    this->registry.view<Animator>().each([&clock](auto animator_entity, auto& animator) {
        
        if (animator.frame_time > (*(animator.frame_durations))[animator.current_frame]) {

            animator.current_frame = (animator.current_frame + 1) % animator.num_frames;
            animator.frame_time = 0;
        }

        animator.frame_time += clock.getDeltaTime();
    });
}

// TODO: only update the textures which had updated animators
void AnimationSystem::updateTextures() {

    this->registry.view<Animation, Texture>().each([](auto animation_entity, const auto& animation, auto& texture) {
        texture.frame_data = animation.animation_data->frames[animation.animator->current_frame];
    });
}

void AnimationSystem::updateIdleAnimations() {

    auto idle_animation_entities = this->registry.view<Animation, Texture, IdleAnimation, Spacial>(entt::exclude<Velocity>);

    for (auto entity : idle_animation_entities) {

        auto [animation, texture, idle_animation, spacial] = idle_animation_entities.get<Animation, Texture, IdleAnimation, Spacial>(entity);

        bool needs_animation_update = animation.animation_data->name != "idle" || animation.animation_data->direction != spacial.direction;

        if (needs_animation_update) {

            animation.animation_data = &(idle_animation.animations[spacial.direction]);

            animation.animator->frame_durations = &(animation.animation_data->frame_durations);
            animation.animator->num_frames = animation.animation_data->num_frames;
            animation.animator->current_frame = 0;
            animation.animator->frame_time = 0;

            texture.frame_data = animation.animation_data->frames[0];
        }
    }
}

void AnimationSystem::updateMoveAnimations() {

    auto move_animation_entities = this->registry.view<Animation, Texture, MoveAnimation, Spacial, Velocity>();

    for (auto entity : move_animation_entities) {

        auto [animation, texture, moveAnimation, spacial] = move_animation_entities.get<Animation, Texture, MoveAnimation, Spacial>(entity);

        std::string previous_animation = animation.animation_data->name;

        bool needs_animation_update = animation.animation_data->name != "move" || animation.animation_data->direction != spacial.direction;

        if (needs_animation_update) {

            animation.animation_data = &(moveAnimation.animations[spacial.direction]);
            animation.animator->frame_durations = &(animation.animation_data->frame_durations);
            animation.animator->num_frames = animation.animation_data->num_frames;

            // You get smoother animations, especially walking, if the walk cycle can continue between direction changes
            // Because of this, on an animation update, the walk-cycle is restarted only after the following conditions
            bool needs_animation_restart = previous_animation != "move" || animation.animator->current_frame >= animation.animation_data->frames.size();

            if (needs_animation_restart) {

                animation.animator->current_frame = 0;
                animation.animator->frame_time = 0;

                texture.frame_data = animation.animation_data->frames[0];

            } else {
                texture.frame_data = animation.animation_data->frames[animation.animator->current_frame];
            }
        }
    }
}