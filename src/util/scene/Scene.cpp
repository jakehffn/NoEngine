#include "Scene.h"

Scene::Scene(SDL_Window* window, Clock* clock, Input* input, CameraController* cameraController) :
    window{ window }, clock{ clock }, input{ input }, cameraControllerPosition{ 0 },
    shaderPrograms{ std::vector<ShaderProgram*>() }, 
    sprites{ std::vector<Sprite>() }, instances{ std::vector<Instance>() } {

        this->cameraControllers = std::vector<CameraController*>{ cameraController };
        this->camera = Camera(this->cameraControllers.at(0));

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

int Scene::addSprite(char* spritePath) {

    Sprite newSprite = Sprite(spritePath);
    this->sprites.push_back(newSprite);
    return this->sprites.size() - 1;
}

int Scene::addInstance(int objID, int shaderProgramID, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) {
    
    Sprite* instanceObject = &(this->sprites.at(objID));

    instances.emplace_back(instanceObject, shaderProgramID, position, scale, rotation);

    return instances.size() - 1;
}

int Scene::addInstance(int objID, int shaderProgramID, glm::vec3 position, glm::vec3 scale) {
    return this->addInstance(objID, shaderProgramID, position, scale, glm::vec3(0, 0, 0));
}

int Scene::addInstance(int objID, int shaderProgramID, glm::vec3 position) {
    return this->addInstance(objID, shaderProgramID, position, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));
}

Instance& Scene::getInstance(int instanceID) {

    assert(instanceID < instances.size());

    return this->instances[instanceID];
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

    for (auto& instance : instances) {

        instance.updateModel();

        int instanceShaderID = instance.getShaderProgramID();

        assert(instanceShaderID < shaderPrograms.size());

        // Use our shader
        ShaderProgram* instanceShader = this->shaderPrograms.at(instanceShaderID);
        GLuint openGLShaderProgramID = instanceShader->getOpenGLShaderProgramID();
        glUseProgram(openGLShaderProgramID);

        glm::mat4 model = instance.getModel();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        instanceShader->renderSetup(model, view, projection);
        
        glBindVertexArray(instance.getSpriteVAO());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, instance.getSpriteTexture());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glDrawArrays(GL_TRIANGLES, 0, 6); 
        glBindVertexArray(0);
    }
}
