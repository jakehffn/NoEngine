#include "LogicObject.h"

LogicObject::LogicObject(glm::vec3 position) : position{ position } {}

void LogicObject::setPosition(glm::vec3 position) {
    this->position = position;
}

void LogicObject::addPosition(glm::vec3 position) {
    this->position += position;
}

glm::vec3 LogicObject::getPosition() const {
    return this->position;
}