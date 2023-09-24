#pragma once

#include "dialog.hpp"
#include "dialog_step.hpp"

#include "text.hpp"
#include "text_manager.hpp"

class DialogBuilder {
public:
    enum Options {
        // A blocking dialog will prevent the player from interacting again while the dialog is open
        DIALOG_BULLDER_BLOCKING = 1,
        DIALOG_BUILDER_FREEZE = 1 << 1
    };
    
    DialogBuilder(entt::registry& registry, float width, uint64_t options=DIALOG_BULLDER_BLOCKING);
    DialogBuilder& text(Text text, float speed = 50.0f);
    DialogBuilder& text(std::u32string text, float speed = 50.0f);
    DialogBuilder& waitForInput();

    Dialog end();
private:
    void addSkipTo(DialogStep* skip_to);

    uint64_t options;
    std::vector<std::shared_ptr<DialogText>> needs_skip_location;

    Dialog dialog;
    entt::registry& registry;
};