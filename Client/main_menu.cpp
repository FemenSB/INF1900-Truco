#include "main_menu.h"
#include "game_view.h"
#include "../Controllers/game_controller.h"
#include <thread>

MainMenu::MainMenu(): WindowView(1280, 720) {}

void MainMenu::LoadMainPageBackground() {
	background_rect_.setSize(sf::Vector2f(screen_width_, screen_height_));
	background_rect_.setFillColor(sf::Color(255, 255, 255, 80)); // half transparent
	background_rect_.setTexture(&(asset_manager_->operator[]("mainPage")));

	main_page_title_.setFont(*asset_manager_->font());	
	main_page_title_.setString("Main menu");
	main_page_title_.setCharacterSize(24); // in pixels, not points!	
	main_page_title_.setFillColor(sf::Color::White);
	main_page_title_.setStyle(sf::Text::Bold);
	main_page_title_.setPosition(20, 0);
}

void MainMenu::LoadMainPageMenu() {
	options_ = { "New Game", "Quit" };
	texts_.resize(2);
	sizes_ = { 28,28 };
	float widthAdjust = (screen_width_ / 2) - 90;
	coords_ = { {widthAdjust,200},{widthAdjust,250} };

	for (int i = 0; i < texts_.size(); i++) {
		texts_[i].setFont(*asset_manager_->font());
		texts_[i].setString(options_[i]);
		texts_[i].setCharacterSize(sizes_[i]);
		texts_[i].setOutlineColor(sf::Color::Black);
		texts_[i].setPosition(coords_[i]);
	}
	texts_[0].setOutlineThickness(3);

	last_winner_.setFont(*asset_manager_->font());
	last_winner_.setString("");
	last_winner_.setCharacterSize(28);
	last_winner_.setFillColor(sf::Color::White);
	last_winner_.setStyle(sf::Text::Bold);
	last_winner_.setPosition(sf::Vector2f(widthAdjust, 300));
}

void MainMenu::Init(const std::string screenName) {
	WindowView::Init(screenName);

	LoadMainPageBackground();
	LoadMainPageMenu();
}

void MainMenu::Loop() {
	// Clear screen
	window_.clear();

	window_.draw(background_rect_);
	window_.draw(main_page_title_);
	window_.draw(last_winner_);

	for (sf::Text text_ : texts_) {
		window_.draw(text_);
	}

	window_.display();
}

void MainMenu::EventLoop(const sf::Event& event) {
	WindowView::EventLoop(event);

	HandleMenuItemsHover(event);

	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		//new game
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (texts_[0].getGlobalBounds().contains(mouse_coord)) {

				window_.setVisible(false);
				DisplayNewGameWindow_();
				window_.setVisible(true);
			}
		}
		break;
	}

	CheckCloseAction(event);
}

void MainMenu::HandleMenuItemsHover(const sf::Event& event) {
	switch (event.type)
	{
	case sf::Event::MouseMoved:
		if (texts_[0].getGlobalBounds().contains(mouse_coord)) {
			texts_[0].setOutlineThickness(3);
			hovered_ = true;
		}
		else if (texts_[0].getOutlineThickness() != 0) {
			hovered_ = false;
			texts_[0].setOutlineThickness(0);
		}

		if (texts_[1].getGlobalBounds().contains(mouse_coord)) {
			texts_[1].setOutlineThickness(3);
			hovered_ = true;
		}
		else if (texts_[1].getOutlineThickness() != 0) {
			texts_[1].setOutlineThickness(0);
			hovered_ = false;
		}
		break;
	}
}

void MainMenu::DisplayNewGameWindow_() {
	auto controller = std::make_shared<GameController>();
	GameView player_1(Characters::PLAYER_1, controller);
	GameView player_2(Characters::PLAYER_2, controller);
	
	player_1.current_player_screen_value.CreateText(18, sf::Vector2f(10, 10), 0, "Tela jogador 1");
	player_2.current_player_screen_value.CreateText(18, sf::Vector2f(10, 10), 0, "Tela jogador 2");

	std::thread t1(&GameView::Start, &player_1, "Player 1 Screen");
	std::thread t2(&GameView::Start, &player_2, "Player 2 Screen");
	t1.join();
	t2.join();
	auto last_winner_text = controller->Winner() == Characters::PLAYER_1 ?
			"Jogador 1 venceu!" : "Jogador 2 venceu!";
	last_winner_.setString(last_winner_text);
}

void MainMenu::CheckCloseAction(const sf::Event& event) {

	//dialog title bar handle
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (texts_[1].getGlobalBounds().contains(mouse_coord)) {
				window_.close();
			}
		}
		break;
	}
}
