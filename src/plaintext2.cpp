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
        textCursor.targetX = p.x();
        textCursor.targetY = p.y();
        textCursor.updateRequest = true;
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
                    textCursor.targetY = -1.0;
                    textCursor.targetX = textCursor.posX;
                    textCursor.updateRequest = true;
                } else {
                    textCursor.index = 0;
                }
                return true;
            } else if (key == GLFW_KEY_DOWN) { // Стрелка вниз
                if (textCursor.row < textBlock.rows-1) {
                    textCursor.row++;
                    textCursor.targetY = -1.0;
                    textCursor.targetX = textCursor.posX;
                    textCursor.updateRequest = true;
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

// Результатом работы функции фактически является значение строки и индекс в тексте
int TextAreaWidget::position2CursorIndex(NVGcontext *ctx, float posX, float posY, NVGtextRow *rows) 
{
    if (posY >= 0) {
        textCursor.row = (int)(posY / textBlock.lineh); 
        if (textCursor.row >= textBlock.rows) {
            textCursor.row = textBlock.rows-1;
        }
    }

    if (posX >= 0) {
        const int maxGlyphs = 1024;
        NVGglyphPosition glyphs[maxGlyphs];
        textBlock.indexes = nvgTextGlyphPositions(ctx, mPos.x(), mPos.y(),
            rows[textCursor.row].start, rows[textCursor.row].end, glyphs, maxGlyphs);

        textBlock.width = nvgTextBounds(ctx, 0, 0, rows[textCursor.row].start, rows[textCursor.row].end, nullptr);
        
        textCursor.index = 0;
        float caretx = glyphs[textCursor.index].x;
        for (int i = 1; i < textBlock.indexes; i++) {
            // std::cout << glyphs[i].x << ",";
            if (std::abs(caretx - posX) > std::abs(glyphs[i].x - posX)) {
                textCursor.index = i;
                caretx = glyphs[textCursor.index].x;
            }
        }

        if (std::abs(caretx - posX) > std::abs(textBlock.width - posX)) 
            textCursor.index = textBlock.indexes;
    }
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
