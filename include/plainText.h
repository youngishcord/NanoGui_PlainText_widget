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
    struct Cursor {
        int row, index, textIndex;
        float posX, posY;

    };

    PlainText(Widget *parent, const std::string &value = "Untitled");

    virtual void draw(NVGcontext *ctx) override;
    
    virtual bool keyboardEvent(int key, int /*scancode*/, int action, int modifiers) override;
    virtual bool keyboardCharacterEvent(unsigned int codepoint) override;

    virtual int _position2CursorIndex(NVGcontext *ctx, float posX, float posY, float *textBound, 
        /*const NVGglyphPosition *glyphs,*/ const NVGtextRow *rows, int size, int nRows, float lineh); // override;

    virtual void _updateCursor(NVGcontext *ctx, int size, NVGtextRow *rows, int nRows, float lineh);

    virtual float _cursorIndex2Position(int index, const NVGglyphPosition *glyphs, int size);
    
    void updateRowText();

protected:
    int mCursorRow = 0;
    float textBound[4];
    int max_rows = 0;
    int max_index = 0;

    const int maxGlyphs = 1024;
    const int maxRows = 128;

    Cursor cursor;

    NVGcontext *ctx;
    NVGtextRow splited_text[128]; // Не дает использовать переменные
    NVGglyphPosition row_text[1024];
};

#endif // PLAINTEXT