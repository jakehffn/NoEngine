#include "create_entity.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void create_entity::VSCodeBackground(entt::registry& registry) {

    const auto background = registry.create();

    Sprite& sprite = registry.emplace<Sprite>(background);
    sprite = create_entity::createSprite("./src/assets/sprites/ScreenShot (76).png");

    registry.emplace<Model>(background, glm::scale(glm::mat4(1), glm::vec3(sprite.width, sprite.height, 0)));
    registry.emplace<Spacial>(background, glm::vec3(0, 0, -.5), glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(sprite.width, sprite.height));
}

void create_entity::Map1Background(entt::registry& registry) {

    const auto background = registry.create();

    Sprite& sprite = registry.emplace<Sprite>(background);
    sprite = create_entity::createSprite("./src/assets/maps/baseMap/baseMap.png");

    registry.emplace<Model>(background, glm::scale(glm::mat4(1), glm::vec3(sprite.width, sprite.height, 0)));
    registry.emplace<Spacial>(background, glm::vec3(0, 0, -.5), glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(sprite.width, sprite.height));
}

void create_entity::Player(entt::registry& registry, glm::vec3 pos) {

    const auto player = registry.create();
    registry.emplace<Model>(player, glm::mat4(1));
    
    registry.emplace<Velocity>(player);
    registry.emplace<Collision>(player, glm::vec2(15, 8), glm::vec2(1, -8));
    registry.emplace<Input>(player, 200.0f);
    registry.emplace<CameraController>(player, 650.0f);

    Sprite& sprite = registry.emplace<Sprite>(player);
    sprite = create_entity::createSprite("./src/assets/sprites/Kid/Kid_IdleDown.png");

    // Patch spacial in for render system to update on start
    registry.emplace<Spacial>(player);
    Spacial initSpacial{pos, glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(sprite.width, sprite.height)};

    registry.patch<Spacial>(player, [initSpacial](auto &spacial) { 
                spacial = initSpacial;
    });

    registry.emplace<Animation>(player, std::vector<int>{0,0,0}); //solve necessity for animations longer than 1 frame        

    Sprite idleUpSprite = create_entity::createSprite("./src/assets/sprites/Kid/Kid_IdleUp.png");
    Sprite moveUpSprite = create_entity::createSprite("./src/assets/sprites/Kid/Kid_MoveUp.png", 4);
    Sprite idleDownSprite = create_entity::createSprite("./src/assets/sprites/Kid/Kid_IdleDown.png");
    Sprite moveDownSprite = create_entity::createSprite("./src/assets/sprites/Kid/Kid_MoveDown.png", 4);
    Sprite idleLeftSprite = create_entity::createSprite("./src/assets/sprites/Kid/Kid_IdleLeft.png");
    Sprite moveLeftSprite = create_entity::createSprite("./src/assets/sprites/Kid/Kid_MoveLeft.png", 4);
    Sprite idleRightSprite = create_entity::createSprite("./src/assets/sprites/Kid/Kid_IdleRight.png");
    Sprite moveRightSprite = create_entity::createSprite("./src/assets/sprites/Kid/Kid_MoveRight.png", 4);

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

void create_entity::BoxHead(entt::registry& registry, glm::vec3 pos) {

    const auto boxHead = registry.create();
    registry.emplace<Model>(boxHead, glm::mat4(10));
    registry.emplace<Input>(boxHead, 675.0f);

    Sprite& sprite = registry.emplace<Sprite>(boxHead);
    sprite = create_entity::createSprite("./src/assets/sprites/BoxHead/BoxHead_IdleDown.png");

    // Patch spacial in for render system to update on start
    registry.emplace<Spacial>(boxHead);
    Spacial initSpacial{pos, glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(sprite.width, sprite.height)};

    registry.patch<Spacial>(boxHead, [initSpacial](auto &spacial) { 
                spacial = initSpacial;
    });

    registry.emplace<Animation>(boxHead, std::vector<int>{0,0,0});//solve necessity for animations longer than 1 frame        

    Sprite idleUpSprite = create_entity::createSprite("./src/assets/sprites/BoxHead/BoxHead_IdleUp.png");
    Sprite moveUpSprite = create_entity::createSprite("./src/assets/sprites/BoxHead/BoxHead_MoveUp.png", 8);
    Sprite idleDownSprite = create_entity::createSprite("./src/assets/sprites/BoxHead/BoxHead_IdleDown.png");
    Sprite moveDownSprite = create_entity::createSprite("./src/assets/sprites/BoxHead/BoxHead_MoveDown.png", 8);
    Sprite idleLeftSprite = create_entity::createSprite("./src/assets/sprites/BoxHead/BoxHead_IdleLeft.png");
    Sprite moveLeftSprite = create_entity::createSprite("./src/assets/sprites/BoxHead/BoxHead_MoveLeft.png", 8);
    Sprite idleRightSprite = create_entity::createSprite("./src/assets/sprites/BoxHead/BoxHead_IdleRight.png");
    Sprite moveRightSprite = create_entity::createSprite("./src/assets/sprites/BoxHead/BoxHead_MoveRight.png", 8);

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

    registry.emplace<SpriteState>(boxHead, stateMap);
}

void create_entity::Bag(entt::registry& registry, glm::vec3 pos) {

    const auto bag = registry.create();

    Sprite& sprite = registry.emplace<Sprite>(bag);
    int numSprites = 5;
    sprite = create_entity::createSprite("./src/assets/sprites/Bag.png", numSprites);

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

void create_entity::CollisionBox(entt::registry& registry, glm::vec2 pos, glm::vec2 dim) {

    const auto collision = registry.create();

    registry.emplace<Spacial>(collision, glm::vec3(pos.x, pos.y, 0), glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), dim);
    registry.emplace<Collision>(collision, dim);
}

void create_entity::TextBox(entt::registry& registry, std::string text) {

    const auto textBox = registry.create();

    registry.emplace<Text>(textBox, text);
    registry.emplace<Spacial>(textBox, glm::vec3(0,220,0), glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(800, 10));
    registry.emplace<Model>(textBox, glm::mat4(1));
}

Sprite create_entity::createSprite(const char* spritesheetPath, int numSprites) {

    Sprite sprite;

    sprite.numSprites = numSprites;
    sprite.texData = glm::vec2(0.0f, 1.0f/numSprites);

     // create texture
    unsigned char* textureData = stbi_load(spritesheetPath, &sprite.width, &sprite.height, &sprite.nColorChannels, STBI_rgb_alpha);

    // create VAO
    float shaderData[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &sprite.VAO);
    
    GLuint dataBuffer;
    glGenBuffers(1, &dataBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shaderData), shaderData, GL_STATIC_DRAW);

    glBindVertexArray(sprite.VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    // Free bound buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);

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