#pragma once

#include "GameObject.h"

#include "Clock.h"

class BackgroundObject : public GameObject {
public:
    BackgroundObject(Clock* clock, GLuint shaderProgramID=0);
    void logic();
private:
    Clock* clock;
};