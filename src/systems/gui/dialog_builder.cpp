#include "dialog_builder.hpp"

DialogBuilder::DialogBuilder(entt::registry& registry, float width) : registry{registry} {
    this->dialog.width = width;
}

DialogBuilder& DialogBuilder::text(Text text, float speed) {
    auto& text_manager = this->registry.ctx().at<TextManager&>();

    auto layout = text_manager.layout(text.text, text.font_family, this->dialog.width);
    
    for (auto row : layout) {
        auto new_text{text};
        new_text.text = row;
        auto new_step = std::make_shared<DialogText>(new_text);
        this->needs_skip_location.push_back(new_step);

        if (!this->dialog.steps.size() == 0) {
            this->dialog.steps.back()->next_step = new_step.get();
        }
        this->dialog.steps.push_back(std::move(new_step));
    }

    return *this;
}

DialogBuilder& DialogBuilder::text(std::u32string text, float speed) {
    return this->text((Text) {text}, speed);
}

DialogBuilder& DialogBuilder::waitForInput() {
    auto new_step = std::make_shared<DialogWaitForInput>();

    this->addSkipTo(new_step.get());

    if (!this->dialog.steps.size() == 0) {
        this->dialog.steps.back()->next_step = new_step.get();
    }
    this->dialog.steps.push_back(std::move(new_step));

    return *this;
}

Dialog DialogBuilder::end() {
    auto new_step = std::make_shared<DialogEnd>();

    this->addSkipTo(new_step.get());

    if (!this->dialog.steps.size() == 0) {
        this->dialog.steps.back()->next_step = new_step.get();
    }
    this->dialog.steps.push_back(std::move(new_step));

    this->dialog.current_step = this->dialog.steps.front().get();

    return this->dialog;
}

void DialogBuilder::addSkipTo(DialogStep* skip_to) {
    if (this->needs_skip_location.size() > 0) {
        for (auto ptr : this->needs_skip_location) {
            ptr->skip_to = skip_to;
        }
        this->needs_skip_location.clear();
    }
}