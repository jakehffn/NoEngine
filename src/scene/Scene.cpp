#include "Scene.h"

Scene::Scene(SDL_Window* window, Clock* clock, Input* input, CameraController* cameraController) :
    window{ window }, clock{ clock }, input{ input }, cameraControllerPosition{ 0 },
    shaderPrograms{ std::vector<ShaderProgram*>() }, 
    sprites{ std::vector<Sprite>() }, logicObjects{ std::vector<LogicObject*>() }, 
    spriteObjects{ std::vector<SpriteObject*>() } {

        this->cameraControllers = std::vector<CameraController*>{ cameraController };
        this->camera = Camera(this->cameraControllers.at(0));

        ShaderProgram* shaderProgram = new BasicShader();
        int shaderID = this->addShaderProgram(shaderProgram);

        // glClearColor(0.0f, 0.4f, 0.4f, 0.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

int Scene::addGameObject(LogicObject* logicObject) {

    this->logicObjects.push_back(logicObject);

    return logicObjects.size() - 1;
}

int Scene::addGameObject(SpriteObject* spriteObject) {

    int spriteID = this->addSprite(spriteObject->getSpritePath());
    spriteObject->setSpriteID(spriteID);
    spriteObject->setScaleBySprite(this->sprites.at(spriteID));

    assert(0 < shaderPrograms.size());

    this->spriteObjects.push_back(spriteObject);

    this->addGameObject((LogicObject*)spriteObject);

    return spriteObjects.size() - 1;
}

LogicObject* Scene::getGameObject(int instanceID) {

    assert(instanceID < logicObjects.size());

    return this->logicObjects[instanceID];
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

void Scene::loop() {

    this->logic();
    this->render();
}

void Scene::logic() {

    for (auto& logicObject : this->logicObjects) {
        logicObject->logic();
    }
}

void Scene::render() {

    input->update();
    camera.update();

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& spriteObject : this->spriteObjects) {
        renderInstance(spriteObject);
    }
}

void Scene::renderInstance(SpriteObject* spriteObject) {

    // printf("Rendering gameObject: %i\n", gameObject->getScale().x);

    spriteObject->updateModel();

    int gameObjectShaderID = spriteObject->getShaderProgramID();

    assert(gameObjectShaderID < shaderPrograms.size());

    // Use shader
    ShaderProgram* gameObjectShader = this->shaderPrograms.at(gameObjectShaderID);
    GLuint openGLShaderProgramID = gameObjectShader->getOpenGLShaderProgramID();
    glUseProgram(openGLShaderProgramID);

    glm::mat4 model = spriteObject->getModel();
    glm::mat4 view = this->camera.getViewMatrix();
    glm::mat4 projection = this->camera.getProjectionMatrix();

    gameObjectShader->renderSetup(model, view, projection);
    
    Sprite& sprite = this->sprites.at(spriteObject->getSpriteID());

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
    
    for (auto& spriteObject : map.getSpriteObjects()) {
        this->addGameObject(spriteObject);
    }

    for (auto& logicObject : map.getLogicObjects()) {
        this->addGameObject(logicObject);
    }

    int newController = this->addCameraController(map.getCameraController());
    this->setCameraController(newController);
}