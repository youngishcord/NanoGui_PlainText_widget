#include <plaintext2.h>
#include <iostream>
#include <regex>

#include <nanogui/opengl.h>
#include <nanogui/nanogui.h>


PlainText2::PlainText2(Widget *parent, const std::string &text) 
	: Widget(parent)
	, text(text) {
	vscroll = new VScrollPanel(this);
	vscroll->setFixedSize(nanogui::Vector2i(100, 100));
	// vscroll->setFixedSize(this->size()); // Не работает поскольку размер задается после работы конструктора

	// vscroll should only have *ONE* child. this is what `wrapper` is for
	auto wrapper = new Widget(vscroll);
	// wrapper->setFixedSize(size());
	wrapper->setLayout(new GridLayout());// defaults: 2 columns
	// vscroll = new VScrollPanel(this);
	// vscroll->setFixedSize(size());
	
	// Widget *wrapper = new Widget(vscroll);
	// wrapper->setFixedSize(size());
	// wrapper->setLayout(new BoxLayout(Orientation::Vertical));

	Button *bt;
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");
	bt = new Button(wrapper, "te");

}

void PlainText2::draw(NVGcontext *ctx) {
	Widget::draw(ctx);

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
