#include "RenderSystem.h"

RenderSystem::RenderSystem(entt::registry& registry) : shaderProgram{ new BasicShader() } {
    // initialize group with empty registry for performance
    auto init = registry.group<Sprite>(entt::get<Model>);
}

void RenderSystem::update(entt::registry& registry) {

    auto sprites = registry.group<Sprite>(entt::get<Model>);

    for (auto entity : sprites) {

        auto [sprite, model] = sprites.get(entity);

        renderSprite(sprite, model);
    }
}

void RenderSystem::renderSprite(Sprite sprite, Model model) {
    // spriteObject->updateModel(); Remember to update somewhere else

    // Use shader
    GLuint openGLShaderProgramID = this->shaderProgram->getOpenGLShaderProgramID();
    glUseProgram(openGLShaderProgramID);

    glm::mat4 view = this->camera.getViewMatrix();
    glm::mat4 projection = this->camera.getProjectionMatrix();

    this->shaderProgram->renderSetup(model.model, view, projection);
    
    glBindVertexArray(sprite.getVAO());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.getTexture());

    // Remove anti-aliasing
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glDrawArrays(GL_TRIANGLES, 0, 6); 
    glBindVertexArray(0);
}