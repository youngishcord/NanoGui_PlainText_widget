/*
Микродока
Я хочу заполнять строки как отдельные блоки текста
Для скролинга можно использовать VScrollPanel
Внутри панели враппер
Внутри враммера виджет, который будет отвечать за размеры текстового поля
и отрисовывать текстовые блоки?
Так будет проще всего растягивать виджет vscollpanel


Текст всегда рисуется на центре своей координаты с поправкой на 
выравнивание текста как на картинках 
docs\images\aling_center_in_a_corner.png
docs\images\aling_left_in_a_corner.png
docs\images\aling_right_in_a_corner.png
Получается необходимо настраивать оффсет для каждого выравнивания в 
индивидуальном порядке

*/

#ifndef PLAINTEXT2
#define PLAINTEXT2

#pragma once

#include <nanogui/widget.h>
#include <nanogui/opengl.h>
#include <cstdio>
#include <iostream>

using namespace nanogui;

// Дока
// https://github.com/memononen/nanovg/blob/master/src/nanovg.h#L625

// Тут будет вся работа с текстом и его отрисовкой 
class TextAreaWidget : public Widget {
public:
    enum class Alignment {
        Left,
        Center,
        Right
    };

    // Структура для описания текущего состояния всего текста
    struct TextBlock
    {
        float l_offset, r_offset, u_offset, d_offset; // Текущие отступы рисования текста
        float width, lineh; // Текущая ширина и высота строки
        int rows, indexes; // Текущее количество строк и символов в строке курсора
        // Color color;
    };

    struct TextCursor {
        int row, index, textIndex; // Строка и индекс в многострочном тексте, 
                                   // полнотекстовый индекс для редактирования
        float posX, posY;          // Положение для рисования курсора
    };

    TextAreaWidget(Widget *parent) : Widget(parent) {
        textCursor = TextCursor(0, 0, 0, 10.0f, 10.0f);
    }

    void draw(NVGcontext *ctx) override {
        Widget::draw(ctx);

        nvgFontSize(ctx, fontSize());
        nvgFontFace(ctx, "sans");
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

        // Высота строки для дальнейшего рисования текста и курсора
        nvgTextMetrics(ctx, nullptr, nullptr, &textBlock.lineh);

        if (focused()) {
            // nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
            // nvgTextAlign(ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
            NVGtextRow rows[1024];
            textBlock.rows = nvgTextBreakLines(ctx, text.c_str(), nullptr, 300.0f-10, rows, 1024);

            for (int i = 0; i < textBlock.rows; i++) {
                nvgText(ctx, mPos.x(), mPos.y()+10+textBlock.lineh*i, rows[i].start, rows[i].end);
                drawTestRect(ctx, mPos.x(), mPos.y()+textBlock.lineh*i, 300-10, textBlock.lineh);
            }

            // Рисование курсора
            const int maxGlyphs = 1024;
            NVGglyphPosition glyphs[maxGlyphs];
            textBlock.indexes = nvgTextGlyphPositions(ctx, mPos.x(), mPos.y(),
                rows[textCursor.row].start, rows[textCursor.row].end, glyphs, maxGlyphs);
            cursorIndex2Position(glyphs, textBlock.indexes, textBlock.rows);

            nvgBeginPath(ctx);
            nvgMoveTo(ctx, textCursor.posX, textCursor.posY);
            nvgLineTo(ctx, textCursor.posX, textCursor.posY + textBlock.lineh);
            nvgStrokeColor(ctx, nvgRGBA(255, 192, 0, 255));
            nvgStrokeWidth(ctx, 1.0f);
            nvgStroke(ctx);
        } else {
            nvgTextBox(ctx, mPos.x(), mPos.y()+10, 300-10, text.c_str(), nullptr);
        }
    }

    void drawTestRect(NVGcontext *ctx, float x, float y, float width, float heigh,
                    Color color = Color(255, 0, 0, 255), int strokeWidth = 2)
    {
        nvgBeginPath(ctx);
        nvgRect(ctx, x, y, width, heigh);
        nvgStrokeColor(ctx, color);
        nvgStrokeWidth(ctx, strokeWidth);
        nvgStroke(ctx);
        nvgClosePath(ctx);
    }   

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers) override;
    virtual bool keyboardCharacterEvent(unsigned int codepoint) override;

    int position2CursorIndex(NVGcontext *ctx, float posX, float posY, float *textBound, const NVGtextRow *rows, int size, int nRows, float lineh);
    float cursorIndex2Position(const NVGglyphPosition *glyphs, int nglyphs, int nRows);

protected:
    bool mEditable = true;

    std::string text = "Of course we can still daisy chain cubic Bézier "
        "curves by adding another curve to our path. Lets add the second pair "
        "of quadratic curves as a single cubic curve to our current path:";
    // int offset = 10;

    TextCursor textCursor;
    TextBlock textBlock;
};

class PlainText2 : public Widget {
public:

    // struct Font {
    //     int fontSize;
        
    // }

    PlainText2(Widget *parent, const std::string &text);

	virtual void draw(NVGcontext *ctx) override;

    bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
    virtual bool keyboardCharacterEvent(unsigned int codepoint) override;

protected:

    std::string text;
    Cursor cursor;

    // scroll panel
    VScrollPanel *vscroll;
    TextAreaWidget* textArea;
};




#endif // PLAINTEXT2