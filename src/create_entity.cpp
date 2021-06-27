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

    registry.emplace<Sprite>(player, "./src/assets/sprites/BoxHead/BoxHead_IdleDown.png");
    registry.emplace<Animation>(player, std::vector<int>{0,0,0});//solve necessity for animations longer than 1 frame        

    Sprite idleUpSprite("./src/assets/sprites/BoxHead/BoxHead_IdleUp.png");
    Sprite moveUpSprite("./src/assets/sprites/BoxHead/BoxHead_MoveUp.png", 8);
    Sprite idleDownSprite("./src/assets/sprites/BoxHead/BoxHead_IdleDown.png");
    Sprite moveDownSprite("./src/assets/sprites/BoxHead/BoxHead_MoveDown.png", 8);
    Sprite idleLeftSprite("./src/assets/sprites/BoxHead/BoxHead_IdleLeft.png");
    Sprite moveLeftSprite("./src/assets/sprites/BoxHead/BoxHead_MoveLeft.png", 8);
    Sprite idleRightSprite("./src/assets/sprites/BoxHead/BoxHead_IdleRight.png");
    Sprite moveRightSprite("./src/assets/sprites/BoxHead/BoxHead_MoveRight.png", 8);

    Animation moveUpAnim{std::vector<int>{0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0}};
    Animation moveAnim{std::vector<int>{0,1,2,3,4,5,6,7}}; 
    Animation idleAnim{std::vector<int>{0,0}};

    std::unordered_map<SpriteStatePair, std::tuple<Sprite, Animation>, pair_hash> stateMap{
        {std::make_pair(entity_c::IDLE, entity_c::UP), std::tuple<Sprite, Animation>(idleUpSprite, idleAnim)},
        {std::make_pair(entity_c::IDLE, entity_c::DOWN), std::tuple<Sprite, Animation>(idleDownSprite, idleAnim)},
        {std::make_pair(entity_c::IDLE, entity_c::LEFT), std::tuple<Sprite, Animation>(idleLeftSprite, idleAnim)},
        {std::make_pair(entity_c::IDLE, entity_c::RIGHT), std::tuple<Sprite, Animation>(idleRightSprite, idleAnim)},
        {std::make_pair(entity_c::MOVING, entity_c::UP), std::tuple<Sprite, Animation>(moveUpSprite, moveAnim)},
        {std::make_pair(entity_c::MOVING, entity_c::DOWN), std::tuple<Sprite, Animation>(moveDownSprite, moveAnim)},
        {std::make_pair(entity_c::MOVING, entity_c::LEFT), std::tuple<Sprite, Animation>(moveLeftSprite, moveAnim)},
        {std::make_pair(entity_c::MOVING, entity_c::RIGHT), std::tuple<Sprite, Animation>(moveRightSprite, moveAnim)}

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