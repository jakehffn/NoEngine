#include "Scene.h"

Scene::Scene(SDL_Window* window, Clock* clock, Input* input, CameraController* cameraController) :
    window{ window }, clock{ clock }, input{ input }, cameraControllerPosition{ 0 },
    shaderPrograms{ std::vector<ShaderProgram*>() }, 
    sprites{ std::vector<Sprite>() }, objectInstances{ std::vector<ObjectInstance>() } {

        this->cameraControllers = std::vector<CameraController*>{ cameraController };
        this->camera = Camera(this->cameraControllers.at(0));

        ShaderProgram* shaderProgram = new BasicShader();
        int shaderID = this->addShaderProgram(shaderProgram);

        // glClearColor(0.0f, 0.4f, 0.4f, 0.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
}

int Scene::addShaderProgram(ShaderProgram* shaderProgram) {

    this->shaderPrograms.push_back(shaderProgram);
    return this->shaderPrograms.size() - 1;
}

int Scene::addSprite(const char* spritePath) {

    Sprite newSprite = Sprite(spritePath);
    this->sprites.push_back(newSprite);
    return this->sprites.size() - 1;
}

int Scene::addObjectInstance(GameObject gameObject) {

    int spriteID = this->addSprite(gameObject.getSpritePath());

    assert(0 < shaderPrograms.size());

    objectInstances.emplace_back(spriteID, this->sprites.at(spriteID), 0, // Default to first shader program
        gameObject.getPos(), glm::vec3(0,0,0));

    return objectInstances.size() - 1;
}

ObjectInstance& Scene::getGameObject(int instanceID) {

    assert(instanceID < objectInstances.size());

    return this->objectInstances[instanceID];
}

int Scene::addCameraController(CameraController* cameraController) {
    
	this->cameraControllers.push_back(cameraController);
    return this->cameraControllers.size() - 1;
}

void Scene::setCameraController(int pos) {

	assert(pos < cameraControllers.size());

	this->cameraControllerPosition = pos;
    this->camera.setController(this->cameraControllers.at(pos));
}

void Scene::nextCameraController() {

	cameraControllerPosition++;

	if (cameraControllerPosition == cameraControllers.size()) {
		cameraControllerPosition = 0;
	}

	setCameraController(cameraControllerPosition);
}

void Scene::render() {

    input->update();
    camera.update();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& gameObject : objectInstances) {
        renderInstance(gameObject);
    }
}

void Scene::renderInstance(ObjectInstance gameObject) {

    gameObject.updateModel();

    int gameObjectShaderID = gameObject.getShaderProgramID();

    assert(gameObjectShaderID < shaderPrograms.size());

    // Use shader
    ShaderProgram* gameObjectShader = this->shaderPrograms.at(gameObjectShaderID);
    GLuint openGLShaderProgramID = gameObjectShader->getOpenGLShaderProgramID();
    glUseProgram(openGLShaderProgramID);

    glm::mat4 model = gameObject.getModel();
    glm::mat4 view = this->camera.getViewMatrix();
    glm::mat4 projection = this->camera.getProjectionMatrix();

    gameObjectShader->renderSetup(model, view, projection);
    
    Sprite& sprite = this->sprites.at(gameObject.getSpriteID());

    glBindVertexArray(sprite.getVAO());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.getTexture());

    // Remove anti-aliasing
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glDrawArrays(GL_TRIANGLES, 0, 6); 
    glBindVertexArray(0);
}

void Scene::loadMapObjects(Map map) {
    
    for (auto& gameObject : map.getGameObjects()) {
        this->addObjectInstance(gameObject);
    }
}