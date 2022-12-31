#include "entities.h"

void entities::Player(entt::registry& registry, glm::vec3 pos) {

    const auto player = registry.create();
    
    
    registry.emplace<Collision>(player, std::vector<glm::vec4>{glm::vec4(12, 8, 3, -8)});
    registry.emplace<PlayerControl>(player, 100.0f);
    registry.emplace<CameraController>(player, 650.0f);

    Texture& texture = registry.emplace<Texture>(player);
    texture = {"./src/assets/sprites/Kid/Kid_IdleDown.png"};

    // Patch spacial in for render system to update on start
    registry.emplace<Spacial>(player);
    Spacial initSpacial{pos, glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(16, 24)};

    registry.patch<Spacial>(player, [initSpacial](auto &spacial) { 
                spacial = initSpacial;
    });

    registry.emplace<Animation>(player, 1.0f/10.0f);        

    Texture idleUpSprite{"./src/assets/sprites/Kid/Kid_IdleUp.png"};
    Texture moveUpSprite = {"./src/assets/sprites/Kid/Kid_MoveUp.png", 4};
    Texture idleDownSprite = {"./src/assets/sprites/Kid/Kid_IdleDown.png"};
    Texture moveDownSprite = {"./src/assets/sprites/Kid/Kid_MoveDown.png", 4};
    Texture idleLeftSprite = {"./src/assets/sprites/Kid/Kid_IdleLeft.png"};
    Texture moveLeftSprite = {"./src/assets/sprites/Kid/Kid_MoveLeft.png", 4};
    Texture idleRightSprite = {"./src/assets/sprites/Kid/Kid_IdleRight.png"};
    Texture moveRightSprite = {"./src/assets/sprites/Kid/Kid_MoveRight.png", 4};

    // IdleAnimation idleAnimation = IdleAnimation{};
    registry.emplace<IdleAnimation>(player, std::unordered_map<DIRECTION, Texture>{
        {UP, idleUpSprite},
        {DOWN, idleDownSprite},
        {LEFT, idleLeftSprite},
        {RIGHT, idleRightSprite}
    });

    registry.emplace<MoveAnimation>(player, std::unordered_map<DIRECTION, Texture>{
        {UP, moveUpSprite},
        {DOWN, moveDownSprite},
        {LEFT, moveLeftSprite},
        {RIGHT, moveRightSprite}
    });
}

void entities::Bag(entt::registry& registry, glm::vec3 pos) {

    const auto bag = registry.create();

    Texture& texture = registry.emplace<Texture>(bag);
    int numFrames = 5;
    texture = {"./src/assets/sprites/Bag.png", numFrames};

    registry.emplace<Model>(bag, glm::mat4(1));

    // Patch spacial in for render system to update on start
    registry.emplace<Spacial>(bag);
    Spacial initSpacial{pos, glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(texture.width/numFrames, texture.height)};

    registry.patch<Spacial>(bag, [initSpacial](auto &spacial) { 
                spacial = initSpacial;
    });

    registry.emplace<Animation>(bag, 1.0f/4.0f);        
}

// void entities::CollisionBox(entt::registry& registry, glm::vec2 pos, glm::vec2 dim) {

//     const auto collision = registry.create();

//     registry.emplace<Spacial>(collision, glm::vec3(pos.x, pos.y, 0), glm::vec3(0, 0, 0), 
//         glm::vec3(1, 1, 1), dim);
//     registry.emplace<Collision>(collision, dim);
// }

void entities::Tree(entt::registry& registry, glm::vec3 pos) {

    const auto tree = registry.create();

    Texture& texture = registry.emplace<Texture>(tree);
    texture = {"./src/assets/sprites/Tree.png"};

    // Patch spacial in for render system to update on start
    registry.emplace<Spacial>(tree);
    Spacial initSpacial{pos, glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(48, 48)};

    registry.patch<Spacial>(tree, [initSpacial](auto &spacial) { 
                spacial = initSpacial;
    });
}

void entities::TextBox(entt::registry& registry, std::string text, bool guiElement) {

    const auto textBox = registry.create();
    registry.emplace<Text>(textBox, text, guiElement);
    // Patch spacial in for render system to update on start
    registry.emplace<Spacial>(textBox);
    Spacial initSpacial{glm::vec3(-100,-100,0), glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(800, 10)};

    registry.emplace<FPSCounter>(textBox);

    registry.patch<Spacial>(textBox, [initSpacial](auto &spacial) { 
                spacial = initSpacial;
    });

    const auto textBox2 = registry.create();
    registry.emplace<Text>(textBox2, "Some really long bit of text which shoyuld make the game lag a lot if this is the issueSome really long bit of text which shoyuld make the game lag a lot if this is the issue", guiElement);
    // Patch spacial in for render system to update on start
    registry.emplace<Spacial>(textBox2);
    Spacial initSpacial2{glm::vec3(-150,-120,0), glm::vec3(0, 0, 0), 
        glm::vec3(1, 1, 1), glm::vec2(800, 10)};

    registry.patch<Spacial>(textBox2, [initSpacial2](auto &spacial) { 
                spacial = initSpacial2;
    });
    
}