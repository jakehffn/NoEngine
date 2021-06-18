#include "GameObject.h"
#include "Input.h"

class PlayerObject : public GameObject {
public:
    PlayerObject(glm::vec3 position, Input* input, 
        GLuint shaderProgramID=0, glm::vec3 rotation=glm::vec3(0));
    void logic();
private:
    Input* input;
};

