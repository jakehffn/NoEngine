#pragma once

#include "Sprite.h"

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Used to have an instance of a specific type of object
class Instance {
public:
    Instance(Sprite* sprite, int shaderProgramID, glm::vec3 position, 
        glm::vec3 rotation=glm::vec3(0));
    Instance(Sprite* sprite, GLuint openGLShaderProgramID, glm::mat4 model);

    void updateModel();
    glm::mat4 getModel();

    void setPosition(glm::vec3 position);
    void addPosition(glm::vec3 position);
    glm::vec3 getPosition();

    void setRotation(glm::vec3 rotation);
    glm::vec3 getRotation();

    void setScale(glm::vec3 scale);
    glm::vec3 getScale();

    int getShaderProgramID();
    void setShaderProgram(int shaderProgramID);

    GLuint getSpriteVAO();
    GLuint getSpriteTexture();
    

private:
    Sprite* sprite;
    glm::mat4 model;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    int shaderProgramID;

    bool needsUpdate;
    const bool isStatic;
};