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
	std::cout << "plaitext keyboardEvent\n";
	// Widget::keyboardEvent(key, scancode, action, modifiers);
	return false;
}

bool PlainText2::keyboardCharacterEvent(unsigned int codepoint) {
	std::cout << "plaitext keyboardCharacterEvent\n";
	// Widget::keyboardCharacterEvent(codepoint);
	std::ostringstream convert;
	convert << (char) codepoint;
	textArea->text.append(convert.str());
	return false;
}


bool TextAreaWidget::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers) {
	// std::cout << "click!\n";
	if (button == GLFW_MOUSE_BUTTON_1 && down) {
		std::cout << p.x() << ", " << p.y() << "\n";
	}
	return false;
}

bool TextAreaWidget::keyboardCharacterEvent(unsigned int codepoint) {
	std::cout << "caracter event\n";
	return false;
}

bool TextAreaWidget::keyboardEvent(int key, int scancode, int action, int modifiers) {
	// Widget::keyboardEvent(key, scancode, action, modifiers);
	std::cout << "keyboard event\n";
	// text.append()
	return false;
}
    // if (mEditable && focused()) {
    //     if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    //         if (key == GLFW_KEY_LEFT) {
    //             if (modifiers == GLFW_MOD_SHIFT) {
    //                 if (mSelectionPos == -1)
    //                     mSelectionPos = mCursorPos;
    //             } else {
    //                 mSelectionPos = -1;
    //             }

    //             if (mCursorPos > 0) {
    //                 mCursorPos--;
    //                 cursor.textIndex--;
    //             } else if (mCursorRow > 0) {
    //                 mCursorRow--;
    //                 updateRowText();
    //                 mCursorPos = max_index; // FIXME: Тут надо пересчитать количество символов на предыдущей строке и ставить это значение
    //                 cursor.textIndex--;
    //             }
    //         } else if (key == GLFW_KEY_RIGHT) {
    //             if (modifiers == GLFW_MOD_SHIFT) {
    //                 if (mSelectionPos == -1)
    //                     mSelectionPos = mCursorPos;
    //             } else {
    //                 mSelectionPos = -1;
    //             }

    //             // Тут срочно нужна проверка на максимальное значение в строке и перемещение курсора к последнему символу
    //             if (mCursorPos < max_index) {
    //                 mCursorPos++;
    //                 cursor.textIndex++;
    //             } else if (mCursorRow < max_rows-1) {
    //                 mCursorRow++;
    //                 mCursorPos = 0;
    //                 cursor.textIndex++;
    //             }
    //         } else if (key == GLFW_KEY_UP) {
    //             // Тут надо доработать селект строк
    //             mSelectionPos = -1;
    //             if (mCursorRow > 0) {
    //                 mCursorRow--;
    //                 updateRowText();
    //                 if (mCursorPos > max_index) {
    //                     mCursorPos = max_index;
    //                 }
    //             }
    //         } else if (key == GLFW_KEY_DOWN) {
    //             // Тут надо доработать селект строк
    //             mSelectionPos = -1;
    //             if (mCursorRow < max_rows-1) {
    //                 mCursorRow++;
    //                 updateRowText();
    //                 if (mCursorPos > max_index) {
    //                     mCursorPos = max_index;
    //                 }
    //             }
    //         } else if (key == GLFW_KEY_HOME) {
    //             if (modifiers == GLFW_MOD_SHIFT) {
    //                 if (mSelectionPos == -1)
    //                     mSelectionPos = mCursorPos;
    //             } else {
    //                 mSelectionPos = -1;
    //             }

    //             mCursorPos = 0;
    //         } else if (key == GLFW_KEY_END) {
    //             if (modifiers == GLFW_MOD_SHIFT) {
    //                 if (mSelectionPos == -1)
    //                     mSelectionPos = mCursorPos;
    //             } else {
    //                 mSelectionPos = -1;
    //             }

    //             mCursorPos = (int) mValueTemp.size();
    //         } else if (key == GLFW_KEY_BACKSPACE) {
    //             if (!deleteSelection()) {
    //                 if (cursor.textIndex > 0) {
    //                     mValueTemp.erase(mValueTemp.begin() + cursor.textIndex - 1);
    //                     mCursorPos--;
    //                     cursor.textIndex--;
    //                 }
    //             }
    //         } else if (key == GLFW_KEY_DELETE) {
    //             if (!deleteSelection()) {
    //                 if (cursor.textIndex < (int) mValueTemp.length())
    //                     mValueTemp.erase(mValueTemp.begin() + cursor.textIndex);
    //             }
    //         } else if (key == GLFW_KEY_ENTER) {
    //             mValueTemp.insert(cursor.textIndex, "\n");
    //             // mCursorPos++;
    //             mCursorRow++;
    //             cursor.textIndex++;
    //             updateRowText();
    //             mCursorPos = 0;
    //             // if (!mCommitted)
    //             //     focusEvent(false);
    //         } else if (key == GLFW_KEY_A && modifiers == SYSTEM_COMMAND_MOD) {
    //             mCursorPos = (int) mValueTemp.length();
    //             mSelectionPos = 0;
    //         } else if (key == GLFW_KEY_X && modifiers == SYSTEM_COMMAND_MOD) {
    //             copySelection();
    //             deleteSelection();
    //         } else if (key == GLFW_KEY_C && modifiers == SYSTEM_COMMAND_MOD) {
    //             copySelection();
    //         } else if (key == GLFW_KEY_V && modifiers == SYSTEM_COMMAND_MOD) {
    //             deleteSelection();
    //             pasteFromClipboard();
    //         }

    //         mValidFormat =
    //             (mValueTemp == "") || checkFormat(mValueTemp, mFormat);
    //     }

        // return true;
    // }

    // return false;
// }
