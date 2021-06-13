#include "Instance.h"

Instance::Instance(Sprite* sprite, int shaderProgramID, glm::vec3 position, 
    glm::vec3 scale /*=glm::vec3(1)*/, glm::vec3 rotation /*=glm::vec3(0)*/) :
        sprite{ sprite }, shaderProgramID{ shaderProgramID },
        position{ position }, rotation{ rotation }, scale{ scale }, 
        needsUpdate{ true }, isStatic{ false } {

    updateModel();
}

Instance::Instance(Sprite* sprite, GLuint openGLShaderProgramID, glm::mat4 model) :
    sprite{ sprite }, model{ model }, 
    position{ glm::vec3() }, rotation{ glm::vec3() }, scale{ glm::vec3() }, 
    needsUpdate{ false }, isStatic{ true } {}

void Instance::updateModel() {

    if (needsUpdate && !isStatic) {

        // Order matters
        this->model = glm::mat4(1);

        glm::mat4 rotate = glm::mat4(1);
        
        rotate = glm::rotate(rotate, this->rotation.x, glm::vec3(1, 0, 0));
        rotate = glm::rotate(rotate, this->rotation.y, glm::vec3(0, 1, 0));
        rotate = glm::rotate(rotate, this->rotation.z, glm::vec3(0, 0, 1));

        glm::mat4 scale = glm::scale(glm::mat4(1), this->scale);

        glm::mat4 translate = glm::translate(glm::mat4(1), this->position);

        this->model = translate * scale * rotate;

        needsUpdate = false;
    }
}

glm::mat4 Instance::getModel() {
    return this->model;
}

void Instance::setPosition(glm::vec3 position) {

    assert(("setPosition() called on static instance", !isStatic));

    this->position = position;
    needsUpdate = true;
}

void Instance::addPosition(glm::vec3 position) {

    assert(("addPosition() called on static instance", !isStatic));

    this->position += position;
    needsUpdate = true;
}

glm::vec3 Instance::getPosition() {
    return this->position;
}

void Instance::setRotation(glm::vec3 rotation) {

    assert(("setRotation() called on static instance", !isStatic));

    this->rotation = rotation;
    needsUpdate = true;
}

glm::vec3 Instance::getRotation() {
    return this->rotation;
}

void Instance::setScale(glm::vec3 scale) {

    assert(("setScale() called on static instance", !isStatic));

    this->scale = scale;
    needsUpdate = true;
}

glm::vec3 Instance::getScale() {
    return this->scale;
}

int Instance::getShaderProgramID() {
    return this->shaderProgramID;
}

GLuint Instance::getSpriteVAO() {
    return this->sprite->getVAO();
}

GLuint Instance::getSpriteTexture() {
    return this->sprite->getTexture();
}