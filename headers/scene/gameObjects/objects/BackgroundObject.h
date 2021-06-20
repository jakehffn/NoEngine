#pragma once

#include "SpriteObject.h"

#include "Clock.h"

class BackgroundObject : public SpriteObject {
public:
    BackgroundObject(Clock* clock, GLuint shaderProgramID=0);
    void logic();
private:
    Clock* clock;
};