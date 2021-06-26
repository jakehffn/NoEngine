#include "create_entity.h"

void create_entity::VSCodeBackground(entt::registry& registry) {

    const auto background = registry.create();
    registry.emplace<Sprite>(background, "./src/assets/sprites/ScreenShot (76).png");
    registry.emplace<Model>(background, glm::mat4(10));
    registry.emplace<Spacial>(background, glm::vec3(0, 0, -.5), glm::vec3(0, 0, 0), 
        glm::vec3(render_c::SPR_SCALE_UP, render_c::SPR_SCALE_UP, 0));
}

void create_entity::Player(entt::registry& registry, glm::vec3 pos) {

    const auto player = registry.create();
    registry.emplace<Model>(player, glm::mat4(10));
    registry.emplace<Spacial>(player, pos, glm::vec3(0, 0, 0), 
        glm::vec3(render_c::SPR_SCALE_UP, render_c::SPR_SCALE_UP, 0));
    registry.emplace<Input>(player, 750.0f);
    registry.emplace<CameraController>(player, 750.0f);

    registry.emplace<Sprite>(player, "./src/assets/sprites/BagHead.png");
    registry.emplace<Animation>(player, std::vector<int>{0,0,0});//solve necessity for animations longer than 1 frame        

    Sprite idleState("./src/assets/sprites/BagHead.png");
    Sprite moveUpState("./src/assets/sprites/Bag.png", 5);
    Sprite moveDownState("./src/assets/sprites/BoxHead_MoveDown.png", 8);

    Animation moveUpAnimation{std::vector<int>{0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0}};
    Animation moveDownAnimation{std::vector<int>{0,1,2,3,4,5,6,7}}; 
    Animation idleAnimation{std::vector<int>{0,0}};

    std::unordered_map<SpriteStatePair, std::tuple<Sprite, Animation>, pair_hash> stateMap{
        {std::make_pair(entity_c::IDLE, entity_c::UP), std::tuple<Sprite, Animation>(idleState, idleAnimation)},
        {std::make_pair(entity_c::IDLE, entity_c::DOWN), std::tuple<Sprite, Animation>(moveDownState, idleAnimation)},
        {std::make_pair(entity_c::IDLE, entity_c::LEFT), std::tuple<Sprite, Animation>(idleState, idleAnimation)},
        {std::make_pair(entity_c::IDLE, entity_c::RIGHT), std::tuple<Sprite, Animation>(idleState, idleAnimation)},
        {std::make_pair(entity_c::MOVING, entity_c::UP), std::tuple<Sprite, Animation>(idleState, idleAnimation)},
        {std::make_pair(entity_c::MOVING, entity_c::DOWN), std::tuple<Sprite, Animation>(moveDownState, moveDownAnimation)},
        {std::make_pair(entity_c::MOVING, entity_c::LEFT), std::tuple<Sprite, Animation>(idleState, idleAnimation)},
        {std::make_pair(entity_c::MOVING, entity_c::RIGHT), std::tuple<Sprite, Animation>(idleState, idleAnimation)}

    };

    registry.emplace<SpriteState>(player, stateMap);
}

void create_entity::Bag(entt::registry& registry, glm::vec3 pos) {

    const auto bag = registry.create();
    registry.emplace<Sprite>(bag, "./src/assets/sprites/Bag.png", 5);
    registry.emplace<Model>(bag, glm::mat4(10));
    registry.emplace<Spacial>(bag, pos, glm::vec3(0, 0, 0), 
        glm::vec3(render_c::SPR_SCALE_UP, render_c::SPR_SCALE_UP, 0));
    registry.emplace<Animation>(bag, std::vector<int>{0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0});        
}