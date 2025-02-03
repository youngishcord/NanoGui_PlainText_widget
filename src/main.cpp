// #include <nanogui/nanogui.h>

// using namespace nanogui;

// class MyApp : public Screen {
// public:
//     MyApp() : Screen(Vector2i(500, 500), "NanoGUI Example") {
//         // Основной виджет вместо окна
//         Widget *mainWidget = new Widget(this);
//         mainWidget->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 10, 10));

//         // Кнопка сверху
//         Button *mainButton = new Button(mainWidget, "Main Button");
//         mainButton->setCallback([] { std::cout << "Main Button Clicked!" << std::endl; });

//         // Прокручиваемая панель
//         VScrollPanel *scrollPanel = new VScrollPanel(mainWidget);
//         scrollPanel->setFixedHeight(300); // Ограничиваем высоту панели

//         // Внутренний контейнер для кнопок
//         Widget *buttonContainer = new Widget(scrollPanel);
//         buttonContainer->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 5, 5));

//         // Добавляем кнопки
//         for (int i = 0; i < 20; ++i) {
//             Button *btn = new Button(buttonContainer, "Button " + std::to_string(i + 1));
//             btn->setCallback([i] { std::cout << "Button " << (i + 1) << " Clicked!" << std::endl; });
//         }

//         performLayout();
//     }
// };

// int main() {
//     nanogui::init();

//     {
//         nanogui::ref<MyApp> app = new MyApp();
//         app->drawAll();
//         app->setVisible(true);
//         nanogui::mainloop();
//     }

//     nanogui::shutdown();
//     return 0;
// }



#include <stdio.h>
#include <iostream>

#include <nanovg.h>
#include <nanogui/nanogui.h>
#include <nanogui/layout.h>
#include <nanogui/window.h>

#include <plainText.h>
#include <plaintext2.h>

class MainApp : public nanogui::Screen {
public:
	MainApp(nanogui::Vector2i screenSize = nanogui::Vector2i(500, 500), nanogui::Vector2i minimalSize = nanogui::Vector2i(400, 400)) 
	: nanogui::Screen(screenSize, "MainApp", true)
	, minimalSize(minimalSize)
	{
		auto *lay = new nanogui::BoxLayout(nanogui::Orientation::Horizontal, nanogui::Alignment::Middle, 0, 0);
		this->setLayout(lay);

		// nanogui::Button *bt = new nanogui::Button(this, "Hello");
		// bt->setCallback([=](){
		// 	std::cout << "Hello!" << "\n";
		// });

		nanogui::TextBox *tb = new nanogui::TextBox(this, "test textbox");
		tb->setEditable(true);
		tb->setFixedSize(nanogui::Vector2i(100, 100));
		tb->setAlignment(PlainText::Alignment::Right);
		// tb->setSpinnable(true);
		// nanogui::IntBox<int> *itb = new nanogui::IntBox<int>(this, 1);
		// itb->setEditable(true);
		// itb->setSpinnable(true);

		// Старый вариант с наследованием
		// PlainText *text = new PlainText(this, "test\n2");
		// text->setFixedSize(nanogui::Vector2i(100, 100));
		// text = new PlainText(this, "test\r\n2");
		// text->setFixedSize(nanogui::Vector2i(100, 100));
		// text->setAlignment(PlainText::Alignment::Center);
		// text = new PlainText(this, "test\n3");
		// text->setFixedSize(nanogui::Vector2i(100, 100));
		// text->setAlignment(PlainText::Alignment::Right);

		PlainText2 *text = new PlainText2(this, "Text\ntext");
		text->setFixedSize(nanogui::Vector2i(300, 300));

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