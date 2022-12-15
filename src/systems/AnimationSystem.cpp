#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(entt::registry& registry) : System(registry) {

    auto initAnimationGroup = registry.group<Animation, Texture>();
}

void AnimationSystem::update() {

    this->updateIdleAnimations();
    this->updateMoveAnimations();

    this->updateAnimations();
}

void AnimationSystem::updateIdleAnimations() {

    auto idleAnimationEntities = this->registry.view<Texture, IdleAnimation, Spacial>(entt::exclude<Velocity>);

    for (auto entity : idleAnimationEntities) {

        auto [idleAnimation, spacial] = idleAnimationEntities.get<IdleAnimation, Spacial>(entity);

        this->registry.replace<Texture>(entity, idleAnimation.animations.at(spacial.direction));
    }
}

void AnimationSystem::updateMoveAnimations() {

    auto moveAnimationEntities = this->registry.view<Texture, MoveAnimation, Spacial, Velocity>();

    for (auto entity : moveAnimationEntities) {

        auto [moveAnimation, spacial] = moveAnimationEntities.get<MoveAnimation, Spacial>(entity);
        
        this->registry.replace<Texture>(entity, moveAnimation.animations.at(spacial.direction));
    }
}

void AnimationSystem::updateAnimations() {

    auto animatedEntities = registry.group<Animation, Texture>();
    Clock& clock = this->registry.ctx().at<Clock&>();

    // Update all entities with animations
    for (auto animatedEntity : animatedEntities) {

        // printf("Animation update\n");
        auto [animation, texture] = animatedEntities.get(animatedEntity);

        animation.frameTime += clock.getDeltaTime();

        if (animation.frameTime > animation.secondsPerFrame) {

            animation.currentFrame = (animation.currentFrame + 1) % texture.numFrames;
            animation.frameTime = animation.frameTime - animation.secondsPerFrame;
        }

        float frameFraction = 1.0/texture.numFrames;

        texture.texData = glm::vec2(animation.currentFrame * frameFraction, (animation.currentFrame + 1) * frameFraction);
    }
}