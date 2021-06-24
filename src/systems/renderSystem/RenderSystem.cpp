#include "RenderSystem.h"

RenderSystem::RenderSystem(entt::registry& registry) : shaderProgram{ new BasicShader() } {
    
    printf("RenderSystem initializer body\n");
    // glClearColor(0.0f, 0.4f, 0.4f, 0.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize group with empty registry for performance
    auto init = registry.group<Sprite>(entt::get<Model, Spacial, Animation>);
}

void RenderSystem::update(entt::registry& registry, Clock clock) {

    // Camera update comes first as sprite rendering relies on camera
    this->updateCamera(registry);
    this->showEntities(registry, clock);
}

void RenderSystem::showEntities(entt::registry& registry, Clock clock) {

    auto animations = registry.view<Animation, Sprite>();

    for (auto entity : animations) {

        // printf("Animation update\n");
        auto [animation, sprite] = animations.get(entity);

        this->updateAnimation(animation, sprite, clock);
    }

    auto sprites = registry.view<Sprite, Model, Spacial>();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto entity : sprites) {

        auto [sprite, model, spacial] = sprites.get(entity);

        this->updateModel(model, sprite, spacial);
        this->renderSprite(sprite, model);
    }
}

void RenderSystem::updateCamera(entt::registry& registry) {

    auto controllers = registry.group<CameraController>(entt::get<Spacial, Sprite>);

    for (auto entity : controllers) {

        auto [cameraController, spacial, sprite] = controllers.get(entity);

        float xOffset = render_consts::SCREEN_WIDTH/2 - sprite.getWidth() * spacial.scale.x * sprite.texData.y / 2;
        float yOffset = render_consts::SCREEN_HEIGHT/2 - sprite.getHeight() * spacial.scale.y / 2;

        glm::vec3 offset(xOffset, yOffset, 0);
        this->camera.setPosition(spacial.pos - offset);
        // this->camera.setPosition(spacial.pos);
    }

    this->camera.update();
}

void RenderSystem::renderSprite(Sprite sprite, Model model) {
    // spriteObject->updateModel(); Remember to update somewhere else

    // printf("Rendering sprite\n");

    // Use shader
    GLuint openGLShaderProgramID = this->shaderProgram->getOpenGLShaderProgramID();
    glUseProgram(openGLShaderProgramID);

    glm::mat4 view = this->camera.getViewMatrix();
    glm::mat4 projection = this->camera.getProjectionMatrix();

    this->shaderProgram->renderSetup(model.model, view, projection, sprite.texData);
    
    glBindVertexArray(sprite.getVAO());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.getTexture());

    // Remove anti-aliasing
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glDrawArrays(GL_TRIANGLES, 0, 6); 
    glBindVertexArray(0);
}

void RenderSystem::updateModel(Model& model,Sprite sprite, Spacial spacial) {

    // Order matters
    model.model = glm::mat4(1.0f);

    glm::mat4 rotate = glm::mat4(1.0f);
    
    rotate = glm::rotate(rotate, spacial.rot.x, glm::vec3(1, 0, 0));
    rotate = glm::rotate(rotate, spacial.rot.y, glm::vec3(0, 1, 0));
    rotate = glm::rotate(rotate, spacial.rot.z, glm::vec3(0, 0, 1));

    glm::vec3 scaleVec = glm::vec3(spacial.scale.x * sprite.getWidth() * sprite.texData.y, 
        spacial.scale.y * sprite.getHeight(), spacial.scale.z);
    glm::mat4 scale = glm::scale(glm::mat4(1), scaleVec);

    glm::mat4 translate = glm::translate(glm::mat4(1), spacial.pos);

    model.model = translate * scale * rotate;
}

void RenderSystem::updateAnimation(Animation& animation, Sprite& sprite, Clock clock) {

    animation.deltaTime += clock.getDeltaTime();

    if (animation.deltaTime > animation.frameSpeed) {
        animation.currAnimFrame = (animation.currAnimFrame + 1) % (animation.frameOrder.size() - 1);
        animation.deltaTime = 0.0f;
    }

    float frameFraction = 1.0/sprite.numSprites;
    float currFrame = animation.frameOrder.at(animation.currAnimFrame);;

    sprite.texData = glm::vec2(currFrame, frameFraction);
}

void RenderSystem::systemState() {
    printf("No state currently\n");
}