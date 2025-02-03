/*
Микродока
Я хочу заполнять строки как отдельные блоки текста
Для скролинга можно использовать VScrollPanel
Внутри панели враппер
Внутри враммера виджет, который будет отвечать за размеры текстового поля
и отрисовывать текстовые блоки?
Так будет проще всего растягивать виджет vscollpanel


Текст всегда рисуется на центре своей координаты с поправкой на выравнивание текста
как на картинках 
docs\images\aling_center_in_a_corner.png
docs\images\aling_left_in_a_corner.png
docs\images\aling_right_in_a_corner.png
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

class PlainText2 : public Widget {
public:
    enum class Alignment {
        Left,
        Center,
        Right
    };

    struct Cursor {
        int row, index, textIndex;
        float posX, posY;
    };

    struct TextBlock
    {
        Vector2i offset;
        int width;
        std::string text;
        // int index;
        // Color color;
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


class TextAreaWidget : public Widget {
public:
    TextAreaWidget(Widget *parent) : Widget(parent) {}

    void draw(NVGcontext *ctx) override {
        Widget::draw(ctx);

        std::string text = "Of course we can still daisy chain cubic Bézier "
        "curves by adding another curve to our path. Lets add the second pair "
        "of quadratic curves as a single cubic curve to our current path:";

        nvgFontSize(ctx, fontSize());
        nvgFontFace(ctx, "sans");
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        // nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        // nvgTextAlign(ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
        // this->absolutePosition()
        // std::cout << absolutePosition() << ", " << mPos.y() << "\n";
        NVGtextRow rows[1024];
        int lines = nvgTextBreakLines(ctx, text.c_str(), nullptr, 300.0f-10, rows, 1024);

        for (int i = 0; i < lines; i++) {
            nvgText(ctx, mPos.x()+10, mPos.y()+10+20*i, rows[i].start, rows[i].end);
            drawTestRect(ctx, mPos.x()+10, mPos.y()+20*i, 300-10, 20);
            
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

protected:
    // int offset = 10;
};

#endif // PLAINTEXT2