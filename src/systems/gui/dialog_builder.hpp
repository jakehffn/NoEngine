#pragma once

#include "dialog.hpp"
#include "dialog_step.hpp"

#include "text.hpp"

class DialogBuilder {
public:
    DialogBuilder& text(Text text, float speed = 50.0f);
    DialogBuilder& text(std::string text, float speed = 50.0f);

    Dialog end();
private:
    Dialog dialog;
};