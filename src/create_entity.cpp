#include "create_entity.h"

void create_entity::addVSCodeBackground(entt::registry& registry) {

    const auto background = registry.create();
    registry.emplace<Sprite>(background, "./src/assets/sprites/ScreenShot (76).png");
    registry.emplace<Model>(background, glm::mat4(10));
    registry.emplace<Spacial>(background, glm::vec3(0, 0, -.5), glm::vec3(0, 0, 0), 
        glm::vec3(render_consts::SPR_SCALE_UP, render_consts::SPR_SCALE_UP, 0));
}

void create_entity::addPlayer(entt::registry& registry, glm::vec3 pos) {

    const auto player = registry.create();
    registry.emplace<Sprite>(player, "./src/assets/sprites/BagHead.png");
    registry.emplace<Model>(player, glm::mat4(10));
    registry.emplace<Spacial>(player, pos, glm::vec3(0, 0, 0), 
        glm::vec3(render_consts::SPR_SCALE_UP, render_consts::SPR_SCALE_UP, 0));
    registry.emplace<Input>(player, 750.0f);
    registry.emplace<CameraController>(player, 750.0f);
}

void create_entity::addBag(entt::registry& registry, glm::vec3 pos) {

    const auto bag = registry.create();
    registry.emplace<Sprite>(bag, "./src/assets/sprites/Bag.png", 5);
    registry.emplace<Model>(bag, glm::mat4(10));
    registry.emplace<Spacial>(bag, pos, glm::vec3(0, 0, 0), 
        glm::vec3(render_consts::SPR_SCALE_UP, render_consts::SPR_SCALE_UP, 0));
    registry.emplace<Animation>(bag, std::vector<int>{0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0});        
}