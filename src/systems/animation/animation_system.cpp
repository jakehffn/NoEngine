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
        
        if (animator.frameTime > (*(animator.frame_durations))[animator.currentFrame]) {

            animator.currentFrame = (animator.currentFrame + 1) % animator.num_frames;
            animator.frameTime = 0;
        }

        animator.frameTime += clock.getDeltaTime();
    });
}

// TODO: only update the textures which had updated animators
void AnimationSystem::updateTextures() {

    this->registry.view<Animation, Texture>().each([](auto animation_entity, const auto& animation, auto& texture) {
        texture.frameData = &(animation.animationData->frames[animation.animator->currentFrame]);
    });
}

void AnimationSystem::updateIdleAnimations() {

    auto idleAnimationEntities = this->registry.view<Animation, Texture, IdleAnimation, Spacial>(entt::exclude<Velocity>);

    for (auto entity : idleAnimationEntities) {

        auto [animation, texture, idleAnimation, spacial] = idleAnimationEntities.get<Animation, Texture, IdleAnimation, Spacial>(entity);

        bool needsAnimationUpdate = animation.animationData->name != "idle" || animation.animationData->direction != spacial.direction;

        if (needsAnimationUpdate) {

            animation.animationData = &(idleAnimation.animations[spacial.direction]);

            animation.animator->frame_durations = &(animation.animationData->frame_durations);
            animation.animator->num_frames = animation.animationData->numFrames;
            animation.animator->currentFrame = 0;
            animation.animator->frameTime = 0;

            texture.frameData = &(animation.animationData->frames[0]);
        }
    }
}

void AnimationSystem::updateMoveAnimations() {

    auto moveAnimationEntities = this->registry.view<Animation, Texture, MoveAnimation, Spacial, Velocity>();

    for (auto entity : moveAnimationEntities) {

        auto [animation, texture, moveAnimation, spacial] = moveAnimationEntities.get<Animation, Texture, MoveAnimation, Spacial>(entity);

        std::string previousAnimation = animation.animationData->name;

        bool needsAnimationUpdate = animation.animationData->name != "move" || animation.animationData->direction != spacial.direction;

        if (needsAnimationUpdate) {

            animation.animationData = &(moveAnimation.animations[spacial.direction]);
            animation.animator->frame_durations = &(animation.animationData->frame_durations);
            animation.animator->num_frames = animation.animationData->numFrames;

            // You get smoother animations, especially walking, if the walk cycle can continue between direction changes
            // Because of this, on an animation update, the walk-cycle is restarted only after the following conditions
            bool needsAnimationRestart = previousAnimation != "move" || animation.animator->currentFrame >= animation.animationData->frames.size();

            if (needsAnimationRestart) {

                animation.animator->currentFrame = 0;
                animation.animator->frameTime = 0;

                texture.frameData = &(animation.animationData->frames[0]);

            } else {
                texture.frameData = &(animation.animationData->frames[animation.animator->currentFrame]);
            }
        }
    }
}