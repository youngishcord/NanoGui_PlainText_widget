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

	Widget *wrapper = new Widget(vscroll);
	wrapper->setLayout(new BoxLayout(Orientation::Vertical));

	// wrapper->setFixedSize(nanogui::Vector2i(300 ,300));
	
	TextAreaWidget* textArea = new TextAreaWidget(wrapper);
	textArea->setFixedWidth(300); // Обязательно надо указывать ширину, иначе она нулевая
	textArea->setFixedHeight(1000);

	// Button *bt;
	// bt = new Button(wrapper, "te");
	// bt->setFixedHeight(200);
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");
	// bt = new Button(wrapper, "te");

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
