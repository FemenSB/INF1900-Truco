#pragma once

#include <SFML/Graphics.hpp>
#include "assets.h"
#include "../Models/constants.h"

class WindowView {
protected:
	AssetsManager* asset_manager_;
	sf::RenderWindow window_;
	unsigned int screen_width_;
	unsigned int screen_height_;

	virtual void Init(const std::string screenName);
	virtual void Loop() = 0;
	virtual void EventLoop(const sf::Event& event);

public:
	sf::Vector2i pos_mouse;
	sf::Vector2f mouse_coord;
	WindowView(unsigned int screen_width, unsigned int screen_height);
	void Start(const std::string screen_name);
	void CheckCloseAction(const sf::Event& event);
};
