#ifndef PLAINTEXT
#define PLAINTEXT

#pragma once

#include <nanogui/widget.h>
#include <nanogui/textbox.h>
#include <nanogui/opengl.h>
#include <cstdio>

using namespace nanogui;

// Дока
// https://github.com/memononen/nanovg/blob/master/src/nanovg.h#L625

class PlainText : public TextBox {
public:
    PlainText(Widget *parent, const std::string &value = "Untitled");

    virtual void draw(NVGcontext *ctx) override;
    
    virtual bool keyboardEvent(int key, int /*scancode*/, int action, int modifiers) override;

    virtual int _position2CursorIndex(float posX, float posY, float lastX, float lastY, 
        const NVGglyphPosition *glyphs, const NVGtextRow *rows, int size, int nRows); // override;
    
};

#endif // PLAINTEXT