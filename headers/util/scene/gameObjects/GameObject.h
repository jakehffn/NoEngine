#pragma once

#include "Sprite.h"

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Used to have an instance of a specific type of object
class GameObject {
public:
    GameObject(int spriteID, const Sprite sprite, int shaderProgramID, 
        glm::vec3 position, glm::vec3 rotation=glm::vec3(0));
    GameObject(int spriteID, GLuint openGLShaderProgramID, glm::mat4 model);

    void updateModel();
    glm::mat4 getModel();

    void setPosition(glm::vec3 position);
    void addPosition(glm::vec3 position);
    glm::vec3 getPosition() const;

    void setRotation(glm::vec3 rotation);
    glm::vec3 getRotation() const;

    void setScale(glm::vec3 scale);
    glm::vec3 getScale() const;

    int getShaderProgramID();
    void setShaderProgram(int shaderProgramID);

    GLuint getSpriteID() const;
    

private:
    glm::mat4 model;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    int spriteID;
    int shaderProgramID;

    bool needsUpdate;
    const bool isStatic;
};