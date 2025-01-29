#ifndef PLAINTEXT2
#define PLAINTEXT2

#pragma once

#include <nanogui/widget.h>
#include <nanogui/opengl.h>
#include <cstdio>

using namespace nanogui;

// Дока
// https://github.com/memononen/nanovg/blob/master/src/nanovg.h#L625

class PlainText2 : public Widget {
public:
    struct Cursor {
        int row, index, textIndex;
        float posX, posY;
    };

    // struct Font {
    //     int fontSize;
        
    // }

    PlainText2(Widget *parent, const std::string &text);

	virtual void draw(NVGcontext *ctx) override;

protected:
    std::string text;
    Cursor cursor;

    // scroll panel
    VScrollPanel *vscroll;
};

#endif // PLAINTEXT2