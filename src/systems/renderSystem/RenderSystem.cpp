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

    printf("Before registry init\n");
    // initialize group with empty registry for performance
    auto init = registry.group<Sprite>(entt::get<Model, Spacial>);
    printf("After registry init\n");
}

void RenderSystem::update(entt::registry& registry) {

    auto sprites = registry.group<Sprite>(entt::get<Model, Spacial>);

    this->camera.update();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto entity : sprites) {

        auto [sprite, model, spacial] = sprites.get(entity);

        updateModel(model, sprite, spacial);
        renderSprite(sprite, model);
    }
}

void RenderSystem::renderSprite(Sprite sprite, Model model) {
    // spriteObject->updateModel(); Remember to update somewhere else

    // printf("Rendering sprite\n");

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

void RenderSystem::updateModel(Model& model,Sprite sprite, Spacial spacial) {

    // Order matters
    model.model = glm::mat4(1.0f);

    glm::mat4 rotate = glm::mat4(1.0f);
    
    rotate = glm::rotate(rotate, spacial.rot.x, glm::vec3(1, 0, 0));
    rotate = glm::rotate(rotate, spacial.rot.y, glm::vec3(0, 1, 0));
    rotate = glm::rotate(rotate, spacial.rot.z, glm::vec3(0, 0, 1));

    glm::vec3 scaleVec = glm::vec3(spacial.scale.x * sprite.getWidth(), 
    spacial.scale.y * sprite.getHeight(), spacial.scale.z);
    glm::mat4 scale = glm::scale(glm::mat4(1), scaleVec);

    glm::mat4 translate = glm::translate(glm::mat4(1), spacial.pos);

    model.model = translate * scale * rotate;
}

void RenderSystem::systemState() {
    printf("No state currently\n");
}