#include "animation_system.h"

AnimationSystem::AnimationSystem(entt::registry& registry) : System(registry),
    idleAnimationObserver{ entt::observer(registry, entt::collector.group<Texture, IdleAnimation>(entt::exclude<Velocity>)) }, 
    moveAnimationObserver{ entt::observer(registry, entt::collector.group<Texture, MoveAnimation, Velocity>()) } {

    auto initAnimationGroup = registry.group<Animation, Texture>();
}

void AnimationSystem::update() {

    this->updateAnimations();
    
    this->updateIdleAnimations();
    this->updateMoveAnimations();
}

void AnimationSystem::updateIdleAnimations() {

    auto idleAnimationEntities = this->registry.view<Texture, Animation, IdleAnimation, Spacial>(entt::exclude<Velocity>);

    for (auto entity : idleAnimationEntities) {

        auto [animation, texture, idleAnimation, spacial] = idleAnimationEntities.get<Animation, Texture, IdleAnimation, Spacial>(entity);

        bool needsAnimationUpdate = animation.animationData.name != "idle" || animation.animationData.direction != spacial.direction;

        if (needsAnimationUpdate) {

            animation.animationData = idleAnimation.animations[spacial.direction];
            texture.frameData = animation.animationData.frames[0];
            animation.currentFrame = 0;
            animation.frameTime = 0;
        }
    }
}

void AnimationSystem::updateMoveAnimations() {

    auto moveAnimationEntities = this->registry.view<Texture, Animation, MoveAnimation, Spacial, Velocity>();

    for (auto entity : moveAnimationEntities) {

        auto [animation, texture, moveAnimation, spacial] = moveAnimationEntities.get<Animation, Texture, MoveAnimation, Spacial>(entity);

        std::string previousAnimation = animation.animationData.name;

        bool needsAnimationUpdate = animation.animationData.name != "move" || animation.animationData.direction != spacial.direction;

        if (needsAnimationUpdate) {

            animation.animationData = moveAnimation.animations[spacial.direction];

            // You get smoother animations, especially walking, if the walk cycle can continue between direction changes
            bool needsAnimationRestart = previousAnimation != "move" || animation.currentFrame >= animation.animationData.frames.size();

            if (needsAnimationRestart) {

                texture.frameData = animation.animationData.frames[0];
                animation.currentFrame = 0;
                animation.frameTime = 0;

            } else {
                texture.frameData = animation.animationData.frames[animation.currentFrame];
            }
        }
    }
}

void AnimationSystem::updateAnimations() {

    auto animatedEntities = registry.group<Animation, Texture>();
    Clock& clock = this->registry.ctx().at<Clock&>();

    // Update all entities with animations
    for (auto animatedEntity : animatedEntities) {

        // printf("Animation update\n");
        auto [animation, texture] = animatedEntities.get(animatedEntity);

        if (animation.frameTime > texture.frameData.duration) {

            animation.currentFrame = (animation.currentFrame + 1) % animation.animationData.numFrames;
            animation.frameTime = 0;

            texture.frameData = animation.animationData.frames[animation.currentFrame];
        }

        animation.frameTime += clock.getDeltaTime();
    }
}