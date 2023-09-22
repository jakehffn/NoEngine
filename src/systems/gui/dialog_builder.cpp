#include "dialog_builder.hpp"

DialogBuilder& DialogBuilder::text(Text text, float speed) {
    auto new_step = std::make_shared<DialogText>(text);

    if (!this->dialog.steps.size() == 0) {
        this->dialog.steps.back()->next_step = new_step.get();
    }
    this->dialog.steps.push_back(std::move(new_step));

    return *this;
}

DialogBuilder& DialogBuilder::text(std::string text, float speed) {
    return this->text((Text) {text}, speed);
}

Dialog DialogBuilder::end() {
    auto new_step = std::make_shared<DialogEnd>();

    if (!this->dialog.steps.size() == 0) {
        this->dialog.steps.back()->next_step = new_step.get();
    }
    this->dialog.steps.push_back(std::move(new_step));

    this->dialog.current_step = this->dialog.steps.front().get();

    return this->dialog;
}