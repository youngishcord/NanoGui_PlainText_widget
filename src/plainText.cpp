#include <plainText.h>
#include <iostream>
#include <regex>

#include <nanogui/opengl.h>

using namespace nanogui;

PlainText::PlainText(Widget *parent, const std::string &value)
	: TextBox(parent, value) {

    // Ставлю по стандарту редактируемость
    setEditable(true);

    // Вращение точно должно быть отключено
    setSpinnable(false);

    // В базе всегда левый текст
    setAlignment(PlainText::Alignment::Left);
}


void PlainText::draw(NVGcontext* ctx) {
    Widget::draw(ctx);

    NVGpaint bg = nvgBoxGradient(ctx,
        mPos.x() + 1, mPos.y() + 1 + 1.0f, mSize.x() - 2, mSize.y() - 2,
        3, 4, Color(255, 32), Color(32, 32));
    NVGpaint fg1 = nvgBoxGradient(ctx,
        mPos.x() + 1, mPos.y() + 1 + 1.0f, mSize.x() - 2, mSize.y() - 2,
        3, 4, Color(150, 32), Color(32, 32));
    NVGpaint fg2 = nvgBoxGradient(ctx,
        mPos.x() + 1, mPos.y() + 1 + 1.0f, mSize.x() - 2, mSize.y() - 2,
        3, 4, nvgRGBA(255, 0, 0, 100), nvgRGBA(255, 0, 0, 50));

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPos.x() + 1, mPos.y() + 1 + 1.0f, mSize.x() - 2,
                   mSize.y() - 2, 3);

    if (mEditable && focused())
        mValidFormat ? nvgFillPaint(ctx, fg1) : nvgFillPaint(ctx, fg2);
    else if (mSpinnable && mMouseDownPos.x() != -1)
        nvgFillPaint(ctx, fg1);
    else
        nvgFillPaint(ctx, bg);

    nvgFill(ctx);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + 0.5f, mSize.x() - 1,
                   mSize.y() - 1, 2.5f);
    nvgStrokeColor(ctx, Color(0, 48));
    nvgStroke(ctx);

    nvgFontSize(ctx, fontSize());
    nvgFontFace(ctx, "sans");
    // Тут надо сделать надстройку для установки текста с по уровню Y
    Vector2i drawPos(mPos.x(), mPos.y() + mSize.y() * 0.5f + 1);

    // Этот параметр отвечает за отступы от краев
    float xSpacing = 5.0f;//mSize.y() * 0.3f;
    float ySpacing = 5.0f;

    float unitWidth = 0;

    float spinArrowsWidth = 0.f;

    switch (mAlignment) {
        case Alignment::Left:
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            drawPos.x() += xSpacing + spinArrowsWidth;
            break;
        case Alignment::Right:
            nvgTextAlign(ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
            drawPos.x() += mSize.x() - unitWidth - xSpacing;
            break;
        case Alignment::Center:
            nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
            drawPos.x() += mSize.x() * 0.5f;
            break;
    }

    nvgFontSize(ctx, fontSize());
    nvgFillColor(ctx, mEnabled && (!mCommitted || !mValue.empty()) ?
        mTheme->mTextColor :
        mTheme->mDisabledTextColor);

    // clip visible text area
    float clipX = mPos.x() + xSpacing + spinArrowsWidth - 1.0f;
    float clipY = mPos.y() + 1.0f;
    float clipWidth = mSize.x() - unitWidth - spinArrowsWidth - 2 * xSpacing + 2.0f;
    float clipHeight = mSize.y() - 3.0f;

    nvgSave(ctx);
    nvgIntersectScissor(ctx, clipX, clipY, clipWidth, clipHeight);

    Vector2i oldDrawPos(drawPos);
    drawPos.x() += mTextOffset;

    if (mCommitted) {
        nvgTextBox(ctx, mPos.x()+xSpacing, mPos.y()+10, mSize.x(), 
            mValue.empty() ? mPlaceholder.c_str() : mValue.c_str(), nullptr);
        // nvgText(ctx, drawPos.x(), drawPos.y(),
        //     mValue.empty() ? mPlaceholder.c_str() : mValue.c_str(), nullptr);
    } else {
        // Получение рамок с размером текстбокса
        const int maxGlyphs = 1024;
        NVGglyphPosition glyphs[maxGlyphs];
        float textBound[4];
        nvgTextBoxBounds(ctx, mPos.x()+xSpacing, mPos.y()+ySpacing, mSize.x()-xSpacing,
            mValueTemp.c_str(), nullptr, textBound);
        // nvgTextBounds(ctx, drawPos.x(), drawPos.y(), mValueTemp.c_str(),
        //               nullptr, textBound);

        // Тут считается количество строк, а в переменной rows сохраняются строки.
        NVGtextRow rows[128];
        int nrows = nvgTextBreakLines(ctx, mValueTemp.c_str(), nullptr, mSize.x()-xSpacing, rows, 128);


        // Высота курсора?
        // float lineh = textBound[3] - textBound[1];
        // float lineh = 20.0f;
        // можно получить высоту строки так
        float lineh;
        nvgTextMetrics(ctx, nullptr, nullptr, &lineh);

        // find cursor positions
        // Тут рассчет количества символов и только 
        int nglyphs =
            nvgTextGlyphPositions(ctx, drawPos.x(), drawPos.y(),
                                  mValueTemp.c_str(), nullptr, glyphs, maxGlyphs);
        // std::cout << nglyphs << "\n";

        _position2CursorIndex(
            mMousePos.x(), mMousePos.y(),
            textBound[2], textBound[3],
            glyphs, rows,
            nglyphs, nrows
        )
        updateCursor(ctx, textBound[2], glyphs, nglyphs);

        // compute text offset
        int prevCPos = mCursorPos > 0 ? mCursorPos - 1 : 0;
        int nextCPos = mCursorPos < nglyphs ? mCursorPos + 1 : nglyphs;
        float prevCX = cursorIndex2Position(prevCPos, textBound[2], glyphs, nglyphs);
        float nextCX = cursorIndex2Position(nextCPos, textBound[2], glyphs, nglyphs);

        if (nextCX > clipX + clipWidth)
            mTextOffset -= nextCX - (clipX + clipWidth) + 1;
        if (prevCX < clipX)
            mTextOffset += clipX - prevCX + 1;

        drawPos.x() = oldDrawPos.x() + mTextOffset;

        // draw text with offset
        // nvgText(ctx, drawPos.x(), drawPos.y(), mValueTemp.c_str(), nullptr);
        nvgTextBox(ctx, mPos.x()+xSpacing, mPos.y()+10, mSize.x()-xSpacing, mValueTemp.c_str(), nullptr);
        // nvgTextBreakLines(ctx,)
        nvgTextBounds(ctx, drawPos.x(), drawPos.y(), mValueTemp.c_str(),
                      nullptr, textBound);

        // recompute cursor positions
        nglyphs = nvgTextGlyphPositions(ctx, drawPos.x(), drawPos.y(),
                mValueTemp.c_str(), nullptr, glyphs, maxGlyphs);
        
        // std::cout << nglyphs << "\n"; 

        // for (int i = 0; i < nrows; i++) {
        //     std::cout << rows[i].end << "\n"; 
        // }

        if (mCursorPos > -1) {
            if (mSelectionPos > -1) {
                float caretx = cursorIndex2Position(mCursorPos, textBound[2],
                                                    glyphs, nglyphs);
                float selx = cursorIndex2Position(mSelectionPos, textBound[2],
                                                  glyphs, nglyphs);

                if (caretx > selx)
                    std::swap(caretx, selx);

                // draw selection
                nvgBeginPath(ctx);
                nvgFillColor(ctx, nvgRGBA(255, 255, 255, 80));
                nvgRect(ctx, caretx, drawPos.y() - lineh * 0.5f, selx - caretx,
                        lineh);
                nvgFill(ctx);
            }

            float caretx = cursorIndex2Position(mCursorPos, textBound[2], glyphs, nglyphs);

            // draw cursor
            nvgBeginPath(ctx);
            nvgMoveTo(ctx, caretx, drawPos.y() - lineh * 0.5f);
            nvgLineTo(ctx, caretx, drawPos.y() + lineh * 0.5f);
            nvgStrokeColor(ctx, nvgRGBA(255, 192, 0, 255));
            nvgStrokeWidth(ctx, 1.0f);
            nvgStroke(ctx);
        }
    }
    nvgRestore(ctx);
}

// Функция пересчета мирового положения в индексы текста для отрисовки курсора и т.д..
int PlainText::_position2CursorIndex(
    float posX, float posY, // Позиция мыши или другого источника
    float lastX, float lastY, // Координаты границы отрисовки
    const NVGglyphPosition *glyphs, // Набор символов
    const NVGtextRow *rows, // Набор строк
    int size, int nRows // Размер текста и количество строк.
    ) 
{
    // я думаю, что тут мы считаем находится ли курсор в области по оси X 
    // используя minx, maxx из NVGtextRow
    // для пересчета Y используем индекс строки и значение высоты линии, которое еще надо передать, но я его уже считал в основной функции.
    // проверяем находимся ли мы в области, если нет то некст область, если да то
    // начинам перебирать символы, пока не найдем более близкий и потом принимаем решение ставить текстовый курсор справа от него или слева от него.
    // примерный алгоритм для пересчета положения
    // я думаю вернуть надо два значения, индекс символа и индекс строки
    return 0;
}

bool PlainText::keyboardEvent(int key, int /* scancode */, int action, int modifiers) {
    if (mEditable && focused()) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            if (key == GLFW_KEY_LEFT) {
                if (modifiers == GLFW_MOD_SHIFT) {
                    if (mSelectionPos == -1)
                        mSelectionPos = mCursorPos;
                } else {
                    mSelectionPos = -1;
                }

                if (mCursorPos > 0)
                    mCursorPos--;
            } else if (key == GLFW_KEY_RIGHT) {
                if (modifiers == GLFW_MOD_SHIFT) {
                    if (mSelectionPos == -1)
                        mSelectionPos = mCursorPos;
                } else {
                    mSelectionPos = -1;
                }

                if (mCursorPos < (int) mValueTemp.length())
                    mCursorPos++;
            } else if (key == GLFW_KEY_HOME) {
                if (modifiers == GLFW_MOD_SHIFT) {
                    if (mSelectionPos == -1)
                        mSelectionPos = mCursorPos;
                } else {
                    mSelectionPos = -1;
                }

                mCursorPos = 0;
            } else if (key == GLFW_KEY_END) {
                if (modifiers == GLFW_MOD_SHIFT) {
                    if (mSelectionPos == -1)
                        mSelectionPos = mCursorPos;
                } else {
                    mSelectionPos = -1;
                }

                mCursorPos = (int) mValueTemp.size();
            } else if (key == GLFW_KEY_BACKSPACE) {
                if (!deleteSelection()) {
                    if (mCursorPos > 0) {
                        mValueTemp.erase(mValueTemp.begin() + mCursorPos - 1);
                        mCursorPos--;
                    }
                }
            } else if (key == GLFW_KEY_DELETE) {
                if (!deleteSelection()) {
                    if (mCursorPos < (int) mValueTemp.length())
                        mValueTemp.erase(mValueTemp.begin() + mCursorPos);
                }
            } else if (key == GLFW_KEY_ENTER) {
                mValueTemp.insert(mCursorPos, "\n");
                mCursorPos++;
                // if (!mCommitted)
                //     focusEvent(false);
            } else if (key == GLFW_KEY_A && modifiers == SYSTEM_COMMAND_MOD) {
                mCursorPos = (int) mValueTemp.length();
                mSelectionPos = 0;
            } else if (key == GLFW_KEY_X && modifiers == SYSTEM_COMMAND_MOD) {
                copySelection();
                deleteSelection();
            } else if (key == GLFW_KEY_C && modifiers == SYSTEM_COMMAND_MOD) {
                copySelection();
            } else if (key == GLFW_KEY_V && modifiers == SYSTEM_COMMAND_MOD) {
                deleteSelection();
                pasteFromClipboard();
            }

            mValidFormat =
                (mValueTemp == "") || checkFormat(mValueTemp, mFormat);
        }

        return true;
    }

    return false;
}