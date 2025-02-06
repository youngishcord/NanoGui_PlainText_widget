#include <plaintext2.h>
#include <iostream>
#include <regex>

#include <nanogui/opengl.h>
#include <nanogui/nanogui.h>



PlainText2::PlainText2(Widget *parent, const std::string &text) 
	: Widget(parent)
	, text(text) {
	vscroll = new VScrollPanel(this);
	vscroll->setFixedSize(nanogui::Vector2i(300, 300));
	
	textArea = new TextAreaWidget(vscroll);
	textArea->setFixedWidth(300); // Обязательно надо указывать ширину, иначе она нулевая
	textArea->setFixedHeight(1000);

	// Button *bt;
	// bt = new Button(wrapper, "te");
	// bt->setFixedHeight(200);
}

void PlainText2::draw(NVGcontext *ctx) {
	Widget::draw(ctx);

	// nvgBeginPath(ctx);
	// nvgRect(ctx, mPos.x(), mPos.y(), 20, 20);
	// nvgStrokeColor(ctx, Color(255, 0, 0, 255));
	// nvgStrokeWidth(ctx, 2);
	// nvgStroke(ctx);
	// nvgClosePath(ctx);

	NVGpaint bg = nvgBoxGradient(ctx,
        mPos.x(), mPos.y(), mSize.x(), mSize.y(),
        3, 4, Color(255, 32), Color(32, 32));


    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPos.x(), mPos.y(), mSize.x(),
                   mSize.y(), 3);
	
	nvgFillPaint(ctx, bg);

	nvgFill(ctx);

	nvgFontSize(ctx, fontSize());
    nvgFontFace(ctx, "sans");


}

bool PlainText2::keyboardEvent(int key, int scancode, int action, int modifiers) {
	// std::cout << "plaitext keyboardEvent\n";
	if (mFocused) {
		if (textArea->keyboardEvent(key, scancode, action, modifiers))
			return true;
	}
	return false;
}

bool PlainText2::keyboardCharacterEvent(unsigned int codepoint) {
	// std::cout << "plaitext keyboardCharacterEvent\n";
	if (mFocused) {
		if (textArea->keyboardCharacterEvent(codepoint))
			return true;
	}
	return false;
}


bool TextAreaWidget::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers) {
	Widget::mouseButtonEvent(p, button, down, modifiers);
	// std::cout << "click!\n";
	if (button == GLFW_MOUSE_BUTTON_1 && down) {
		std::cout << p.x() << ", " << p.y() << "\n";
	}

	return false;
}

bool TextAreaWidget::keyboardCharacterEvent(unsigned int codepoint) {
	std::cout << "caracter event\n";
	std::ostringstream convert;
	convert << (char) codepoint;
	text.append(convert.str());
	return true;
	// return false;
}

bool TextAreaWidget::keyboardEvent(int key, int scancode, int action, int modifiers) {
	// Widget::keyboardEvent(key, scancode, action, modifiers);
	// std::cout << "keyboard event\n";
	if (mEditable && focused()) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            if (key == GLFW_KEY_LEFT) { // Стрелка влево
                if (textCursor.index > 0) {
                    textCursor.index--;
                } else if (textCursor.row > 0) {
                    textCursor.row--;
                    // Тут надо как то перекидывать курсор в конец строки
                    textCursor.index = MAXINT;
                }
                return true;
            } else if (key == GLFW_KEY_RIGHT) { // Стрелка вправо
                if (textCursor.index < textBlock.indexes) {
                    textCursor.index++;
                } else if (textCursor.row < textBlock.rows-1) {
                    textCursor.row++;
                    textCursor.index = 0;
                }
                return true;
            } else if (key == GLFW_KEY_UP) { // Стрелка вверх
                if (textCursor.row > 0) {
                    textCursor.row--;
                } else {
                    textCursor.index = 0;
                }
                return true;
            } else if (key == GLFW_KEY_DOWN) { // Стрелка вниз
                if (textCursor.row < textBlock.rows-1) {
                    textCursor.row++;
                } else {
                    textCursor.index = textBlock.indexes;
                }
                return true;
            }
        }
    }
	return false;
}

// Метод для обновления положения курсора в
// void TextAreaWidget::updateCursor()

int TextAreaWidget::position2CursorIndex(NVGcontext *ctx, float posX, float posY, float *textBound, 
    const NVGtextRow *rows, int size, int nRows, float lineh) 
{
    textCursor.row = (int)((posY - textBound[1]) / lineh);

    // Тут надо делать поправку на выравнивание текста но это позже

    return 0;
}

float TextAreaWidget::cursorIndex2Position(const NVGglyphPosition *glyphs, int nglyphs, int nRows) {
    // textCursor.posX = 0;
    if (textCursor.index >= nglyphs){
        textCursor.posX = glyphs[nglyphs-1].maxx; // last character
        textCursor.index = nglyphs;
    } else {
        textCursor.posX = glyphs[textCursor.index].x;
    }

    if (textCursor.row < nRows){
        textCursor.posY = textCursor.row * textBlock.lineh;
    } else {
        textCursor.row = textBlock.rows-1;
    }

    return textCursor.posX;
}
