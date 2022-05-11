#include "entities.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void entities::Player(entt::registry& registry, glm::vec3 pos) {

    const auto player = registry.create();
    registry.emplace<Model>(player, glm::mat4(1));
    
    registry.emplace<Velocity>(player);
    registry.emplace<Collision>(player, std::vector<glm::vec4>{glm::vec4(12, 8, 3, -8)});
    registry.emplace<Input>(player, 100.0f);
    registry.emplace<CameraController>(player, 650.0f);

    Sprite& sprite = registry.emplace<Sprite>(player);
    sprite = entities::createSprite("./src/assets/sprites/Kid/Kid_IdleDown.png");

    // Patch spacial in for render system to update on start
    registry.emplace<Spacial>(player);
    Spacial initSpacial{pos, glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(sprite.width, sprite.height)};

    registry.patch<Spacial>(player, [initSpacial](auto &spacial) { 
                spacial = initSpacial;
    });

    registry.emplace<Animation>(player, std::vector<int>{0,0,0}); //solve necessity for animations longer than 1 frame        

    Sprite idleUpSprite = entities::createSprite("./src/assets/sprites/Kid/Kid_IdleUp.png");
    Sprite moveUpSprite = entities::createSprite("./src/assets/sprites/Kid/Kid_MoveUp.png", 4);
    Sprite idleDownSprite = entities::createSprite("./src/assets/sprites/Kid/Kid_IdleDown.png");
    Sprite moveDownSprite = entities::createSprite("./src/assets/sprites/Kid/Kid_MoveDown.png", 4);
    Sprite idleLeftSprite = entities::createSprite("./src/assets/sprites/Kid/Kid_IdleLeft.png");
    Sprite moveLeftSprite = entities::createSprite("./src/assets/sprites/Kid/Kid_MoveLeft.png", 4);
    Sprite idleRightSprite = entities::createSprite("./src/assets/sprites/Kid/Kid_IdleRight.png");
    Sprite moveRightSprite = entities::createSprite("./src/assets/sprites/Kid/Kid_MoveRight.png", 4);

    Animation moveAnim{std::vector<int>{0,1,2,3}}; 
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

void entities::Bag(entt::registry& registry, glm::vec3 pos) {

    const auto bag = registry.create();

    Sprite& sprite = registry.emplace<Sprite>(bag);
    int numSprites = 5;
    sprite = entities::createSprite("./src/assets/sprites/Bag.png", numSprites);

    registry.emplace<Model>(bag, glm::mat4(1));

    // Patch spacial in for render system to update on start
    registry.emplace<Spacial>(bag);
    Spacial initSpacial{pos, glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(sprite.width/numSprites, sprite.height)};

    registry.patch<Spacial>(bag, [initSpacial](auto &spacial) { 
                spacial = initSpacial;
    });

    registry.emplace<Animation>(bag, std::vector<int>{0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0});        
}

// void entities::CollisionBox(entt::registry& registry, glm::vec2 pos, glm::vec2 dim) {

//     const auto collision = registry.create();

//     registry.emplace<Spacial>(collision, glm::vec3(pos.x, pos.y, 0), glm::vec3(0, 0, 0), 
//         glm::vec3(1, 1, 1), dim);
//     registry.emplace<Collision>(collision, dim);
// }

void entities::Tree(entt::registry& registry, glm::vec3 pos) {

    const auto tree = registry.create();

    Sprite& sprite = registry.emplace<Sprite>(tree);
    sprite = entities::createSprite("./src/assets/sprites/Tree.png");

    registry.emplace<Model>(tree, glm::mat4(1));

    // Patch spacial in for render system to update on start
    registry.emplace<Spacial>(tree);
    Spacial initSpacial{pos, glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(sprite.width, sprite.height)};

    registry.patch<Spacial>(tree, [initSpacial](auto &spacial) { 
                spacial = initSpacial;
    });
}

void entities::TextBox(entt::registry& registry, std::string text, bool guiElement) {

    const auto textBox = registry.create();

    registry.emplace<Text>(textBox, text, guiElement);
    registry.emplace<Spacial>(textBox, glm::vec3(0,220,0), glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(800, 10));
    registry.emplace<Model>(textBox, glm::mat4(1));
}

void entities::TileEntity(entt::registry& registry, std::tuple<int, int> pos, int id, std::vector<glm::vec4> collisions) {

    const auto tile = registry.create();

    glm::vec3 position = glm::vec3(16)*glm::vec3(std::get<0>(pos), std::get<1>(pos), 0);

    registry.emplace<Tile>(tile, id);
    registry.emplace<Collision>(tile, collisions);
    registry.emplace<Spacial>(tile, position, glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(0, 0));

}

void entities::TileEntity(entt::registry& registry, std::tuple<int, int> pos, int id) {

    const auto tile = registry.create();

    glm::vec3 position = glm::vec3(std::get<0>(pos), std::get<1>(pos), 0);

    registry.emplace<Tile>(tile, id);
    registry.emplace<Spacial>(tile, glm::vec3(16)*position, glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(0, 0));
}

Sprite entities::createSprite(const char* spritesheetPath, int numSprites) {

    Sprite sprite;

    sprite.numSprites = numSprites;
    sprite.texData = glm::vec2(0.0f, 1.0f/numSprites);

    // create texture
    unsigned char* textureData = stbi_load(spritesheetPath, &sprite.width, &sprite.height, &sprite.nColorChannels, STBI_rgb_alpha);

    glGenTextures(1, &sprite.texture);
    glBindTexture(GL_TEXTURE_2D, sprite.texture);

    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite.width, sprite.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture\n");
    }

    stbi_image_free(textureData);

    return sprite;
}