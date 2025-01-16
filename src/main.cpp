#include <stdio.h>
#include <iostream>

#include <nanovg.h>
#include <nanogui/nanogui.h>
#include <nanogui/layout.h>
#include <nanogui/window.h>


class MainApp : public nanogui::Screen {
public:
	MainApp(nanogui::Vector2i screenSize = nanogui::Vector2i(500, 500), nanogui::Vector2i minimalSize = nanogui::Vector2i(400, 400)) 
	: nanogui::Screen(screenSize, "MainApp", true)
	, minimalSize(minimalSize)
	{
		auto *lay = new nanogui::BoxLayout(nanogui::Orientation::Horizontal, nanogui::Alignment::Middle, 0, 0);
		this->setLayout(lay);

		nanogui::Button *bt = new nanogui::Button(this, "Hello");
		bt->setCallback([=](){
			std::cout << "Hello!" << "\n";
		});

		performLayout();
		resizeEvent(this->size());
	}

	virtual bool resizeEvent(const nanogui::Vector2i &size) override {
		if (Screen::resizeEvent(size)) {
			return true;
			performLayout();
		}
		performLayout();
		return false;
	}

	virtual void draw(NVGcontext *ctx) {
		Screen::draw(ctx);
	}
private:
	nanogui::Vector2i minimalSize;
	nanogui::Widget *mainFrame;
	GLFWwindow *window;
};



int main() {
	std::cout << "Begin programm\n";

	try {
		nanogui::init();

		{
			nanogui::ref<MainApp> app = new MainApp();
			app->drawAll();
			app->setVisible(true);
			nanogui::mainloop();
		}

		nanogui::shutdown();
	} catch (const std::runtime_error &e) {
		std::string err_msg = std::string("Error: ") + std::string(e.what());
		        #if defined(_WIN32)
            MessageBoxA(nullptr, err_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
        #else
            std::cerr << error_msg << endl;
        #endif
        return -1;
	}

	return 0;
}