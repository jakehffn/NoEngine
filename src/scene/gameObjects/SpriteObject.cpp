#include "SpriteObject.h"

SpriteObject::SpriteObject(const char* spritePath, glm::vec3 position, 
    int shaderProgramID /*=0*/, glm::vec3 rotation /*=glm::vec3(0)*/) :
        LogicObject(position), spritePath{ spritePath }, 
        shaderProgramID{ shaderProgramID }, rotation{ rotation }, 
        needsUpdate{ false }, isStatic{ false } {}

void SpriteObject::updateModel() {

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

glm::mat4 SpriteObject::getModel() {
    return this->model;
}

void SpriteObject::setPosition(glm::vec3 position) {

    assert(("setPosition() called on static instance", !isStatic));

    LogicObject::setPosition(position);
    needsUpdate = true;
}

void SpriteObject::addPosition(glm::vec3 position) {

    assert(("addPosition() called on static instance", !isStatic));

    LogicObject::addPosition(position);
    needsUpdate = true;
}

void SpriteObject::setRotation(glm::vec3 rotation) {

    assert(("setRotation() called on static instance", !isStatic));

    this->rotation = rotation;
    needsUpdate = true;
}

glm::vec3 SpriteObject::getRotation() const {
    return this->rotation;
}

void SpriteObject::setScale(glm::vec3 scale) {

    assert(("setScale() called on static instance", !isStatic));

    this->scale = scale;
    needsUpdate = true;
}

glm::vec3 SpriteObject::getScale() const {
    return this->scale;
}

int SpriteObject::getShaderProgramID() {
    return this->shaderProgramID;
}

void SpriteObject::setShaderProgram(int shaderProgramID) {
    this->shaderProgramID = shaderProgramID;
}

GLuint SpriteObject::getSpriteID() const {
    return this->spriteID;
}

void SpriteObject::setSpriteID(int spriteID) {
    this->spriteID = spriteID;
}

const char* SpriteObject::getSpritePath() const {
    return this->spritePath;
}

void SpriteObject::setScaleBySprite(const Sprite sprite) {

    printf("%i\n", sprite.getHeight());
    this->scale = glm::vec3(sprite.getWidth() * render_consts::SPR_SCALE_UP, 
            sprite.getHeight() * render_consts::SPR_SCALE_UP, 1);

    this->needsUpdate = true;
    this->updateModel();
};
