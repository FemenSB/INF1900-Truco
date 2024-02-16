#include <iostream>
#include <SFML/Graphics.hpp>
#include "assets.h"
#include "text.h"
#include "card_view.h"
#include "deck_view.h"
#include "window_view.h"
#include "../Models/game_status.h"
#include "../Models/constants.h"
#include "../Controllers/game_controller.h"
#include "../Controllers/game_status_observer.h"
#include <mutex>

class GameView : public WindowView, public GameStatusDirtObserver
{
private:
	static std::mutex draw_mutex_;

	sf::RectangleShape drop_area_;
	DeckView DeckOfCards;
	CardView turned_up;
	CardView selected_card_;
	std::vector<CardView> playable_cards_;
	std::vector<CardView> played_cards_;
	
	Text current_hand_text_;
	Text current_hand_value_;

	Text truco_request_button_;
	Text accept_truco_request_;
	Text reject_truco_request_;
	Text enraise_truco_request_;

	Text current_player_text_;
	Text current_player_value_;
	Text hand_value_text_;

	Text first_hand_wins_;
	Text first_hand_wins_value_;
	Text second_hand_wins_;
	Text second_hand_wins_value_;

	Text score_text_;
	Text pair_player_1_;
	Text pair_player_1_points_;
	Text pair_player_2_;
	Text pair_player_2_points_;
	
	sf::RectangleShape new_game_background_rect_;

	void UpdateGameStatus_(const GameStatus &status);
	void RenderGameStatus_();
	void Init(const std::string screenName) override;
	void CreateDisplayTexts_();
	void Loop() override;
	bool IsTrucoRequested_();
	void EventLoop(const sf::Event& event) override;

	void CheckTrucoActions_();

	Characters self_character_;
	std::shared_ptr<GameController> game_controller_;
public:
	Text current_player_screen_value;

	GameView(Characters character,
		std::shared_ptr<GameController> game_controller);
	~GameView();

	void DisplayGameStatus(const GameStatus& status);
};
