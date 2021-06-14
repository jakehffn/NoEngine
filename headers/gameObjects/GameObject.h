#pragma once

#include "Sprite.h"
#include "Instance.h"
#include "Scene.h"

class GameObject {
public:
    GameObject(Scene* scene, const char* spritePath);

protected:
    Scene* scene;
    const char* spritePath;
};