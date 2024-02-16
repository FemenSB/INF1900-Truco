#include <iostream>
#include <SFML/Graphics.hpp>
#include "assets.h"
#include "window_view.h"

class MainMenu : public WindowView
{
private:
	sf::RectangleShape background_rect_;
	sf::Text main_page_title_;
	std::vector<const char*> options_;
	std::vector<sf::Vector2f> coords_;
	std::vector<sf::Text> texts_;
	std::vector<std::size_t> sizes_;
	sf::Text last_winner_;
	bool hovered_;

	void DisplayNewGameWindow_();

public:
	MainMenu();
	void LoadMainPageBackground();
	void LoadMainPageMenu();
	void Init(const std::string screen_name) override;
	void Loop() override;
	void EventLoop(const sf::Event& event) override;

	void CheckCloseAction(const sf::Event& event);
	void HandleMenuItemsHover(const sf::Event& event);
};
