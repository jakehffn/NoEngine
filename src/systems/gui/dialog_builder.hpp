#pragma once

#include "dialog.hpp"
#include "dialog_step.hpp"

#include "text.hpp"
#include "text_manager.hpp"

class DialogBuilder {
public:
    DialogBuilder(entt::registry& registry, float width);
    DialogBuilder& text(Text text, float speed = 50.0f);
    DialogBuilder& text(std::u32string text, float speed = 50.0f);
    DialogBuilder& waitForInput();

    Dialog end();
private:
    void addSkipTo(DialogStep* skip_to);
    std::vector<std::shared_ptr<DialogText>> needs_skip_location;

    Dialog dialog;
    entt::registry& registry;
};