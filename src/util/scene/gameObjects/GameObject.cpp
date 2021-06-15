#include "GameObject.h"

GameObject::GameObject(int spriteID, const Sprite sprite, int shaderProgramID, 
    glm::vec3 position, glm::vec3 rotation /*=glm::vec3(0)*/) :
        spriteID{ spriteID }, shaderProgramID{ shaderProgramID },
        position{ position }, rotation{ rotation }, 
        needsUpdate{ true }, isStatic{ false } {

        this->scale = glm::vec3(sprite.getWidth() * render_consts::SPR_SCALE_UP, 
            sprite.getHeight() * render_consts::SPR_SCALE_UP, 1);

        updateModel();
}

GameObject::GameObject(int spriteID, GLuint openGLShaderProgramID, glm::mat4 model) :
    spriteID{ spriteID }, model{ model }, 
    position{ glm::vec3() }, rotation{ glm::vec3() }, scale{ glm::vec3() }, 
    needsUpdate{ false }, isStatic{ true } {}

void GameObject::updateModel() {

    if (needsUpdate && !isStatic) {

        // Order matters
        this->model = glm::mat4(1.0f);

        glm::mat4 rotate = glm::mat4(1.0f);
        
        rotate = glm::rotate(rotate, this->rotation.x, glm::vec3(1, 0, 0));
        rotate = glm::rotate(rotate, this->rotation.y, glm::vec3(0, 1, 0));
        rotate = glm::rotate(rotate, this->rotation.z, glm::vec3(0, 0, 1));

        glm::mat4 scale = glm::scale(glm::mat4(1), this->scale);

        glm::mat4 translate = glm::translate(glm::mat4(1), this->position);

        this->model = translate * scale * rotate;

        needsUpdate = false;
    }
}

glm::mat4 GameObject::getModel() {
    return this->model;
}

void GameObject::setPosition(glm::vec3 position) {

    assert(("setPosition() called on static instance", !isStatic));

    this->position = position;
    needsUpdate = true;
}

void GameObject::addPosition(glm::vec3 position) {

    assert(("addPosition() called on static instance", !isStatic));

    this->position += position;
    needsUpdate = true;
}

glm::vec3 GameObject::getPosition() const {
    return this->position;
}

void GameObject::setRotation(glm::vec3 rotation) {

    assert(("setRotation() called on static instance", !isStatic));

    this->rotation = rotation;
    needsUpdate = true;
}

glm::vec3 GameObject::getRotation() const {
    return this->rotation;
}

void GameObject::setScale(glm::vec3 scale) {

    assert(("setScale() called on static instance", !isStatic));

    this->scale = scale;
    needsUpdate = true;
}

glm::vec3 GameObject::getScale() const {
    return this->scale;
}

int GameObject::getShaderProgramID() {
    return this->shaderProgramID;
}

void GameObject::setShaderProgram(int shaderProgramID) {
    this->shaderProgramID = shaderProgramID;
}

GLuint GameObject::getSpriteID() const {
    return this->spriteID;
}
