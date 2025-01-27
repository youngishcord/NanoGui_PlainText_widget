#ifndef PLAINTEXT
#define PLAINTEXT

#pragma once

#include <nanogui/widget.h>
#include <nanogui/textbox.h>
#include <cstdio>

using namespace nanogui;

class PlainText : public TextBox {
public:
    PlainText(Widget *parent, const std::string &value = "Untitled");

    virtual void draw(NVGcontext *ctx) override;
    virtual bool keyboardEvent(int key, int /*scancode*/, int action, int modifiers);
};

#endif // PLAINTEXT