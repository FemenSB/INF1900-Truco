#include "window_view.h"

WindowView::WindowView(unsigned int screen_width, unsigned int screen_height)
	: screen_width_(screen_width), screen_height_(screen_height),
	asset_manager_(AssetsManager::GetInstance()) {}

void WindowView::Init(const std::string screenName) {
	window_.create(sf::VideoMode(screen_width_, screen_height_), screenName, sf::Style::Default);
}

void WindowView::Start(const std::string screenName) {
	Init(screenName);
	while (window_.isOpen()) {
		Loop();
		sf::Event event;
		while (window_.pollEvent(event)) {
			EventLoop(event);
			CheckCloseAction(event);
		}
	}
}

void WindowView::EventLoop(const sf::Event& event) {
	pos_mouse = sf::Mouse::getPosition(window_);
	mouse_coord = window_.mapPixelToCoords(pos_mouse);
	CheckCloseAction(event);
}

void WindowView::CheckCloseAction(const sf::Event& event) {
	//dialog title bar handle
	switch (event.type)
	{
	case sf::Event::Closed:
		window_.close();
		break;

	case sf::Event::KeyPressed:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window_.close();
		}
		break;
	}
}
