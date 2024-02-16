#include "game_view.h"

#define INITIAL_CARD_X_POSITION 421
#define INITIAL_CARD_Y_POSITION 600

std::mutex GameView::draw_mutex_;

// Turned_up need to be initialized during construction, so we initialize it as an arbitrary card.
// It's value will be correctly set before rendering
GameView::GameView(Characters character,
	std::shared_ptr<GameController> game_controller)
	: WindowView(1280, 720),
	turned_up(SuitsEnum::SPADES, 1),
	selected_card_(SuitsEnum::SPADES, 1),
	self_character_(character),
	game_controller_(game_controller) {}

GameView::~GameView() {
	game_controller_->RemoveStatusDirtObserver(this);
}

void GameView::Init(const std::string screenName) {
	WindowView::Init(screenName);

	DeckOfCards.Init();

	drop_area_.setSize(sf::Vector2f(400.0f, 250.0f));
	drop_area_.setOrigin(drop_area_.getSize().x / 2, drop_area_.getSize().y / 2);
	drop_area_.setPosition(650, 350);

	CreateDisplayTexts_();

	drop_area_.setFillColor(sf::Color(255, 237, 217, 100));
	drop_area_.setOutlineThickness(3);

	new_game_background_rect_.setSize(sf::Vector2f(screen_width_, screen_height_));
	//dark green color
	new_game_background_rect_.setFillColor(sf::Color(6, 104, 57, 100));

	//set deck in correct position
	DeckOfCards.deck_rect.setPosition(417, 281);

	game_controller_->AddStatusDirtObserver(this);
}

void GameView::CreateDisplayTexts_()
{
	current_player_text_.CreateText(18, sf::Vector2f(600, 20), sf::Text::Bold, "Jogador atual: ");
	current_player_value_.CreateText(18, sf::Vector2f(740, 20));

	current_hand_text_.CreateText(18, sf::Vector2f(600, 50), sf::Text::Bold, "Pontos Valendo: ");
	current_hand_value_.CreateText(18, sf::Vector2f(780, 50), sf::Text::Bold, "1");

	first_hand_wins_.CreateText(12, sf::Vector2f(850, 25), sf::Text::Bold, "Jog. 1 quedas ganhas: ");
	first_hand_wins_value_.CreateText(12, sf::Vector2f(1000, 25), 0, " - ");

	second_hand_wins_.CreateText(12, sf::Vector2f(850, 55), sf::Text::Bold, "Jog. 2 Quedas ganhas: ");
	second_hand_wins_value_.CreateText(12, sf::Vector2f(1000, 55), 0, " - ");

	score_text_.CreateText(18, sf::Vector2f(1090, 20), sf::Text::Bold, "Pontuação atual");

	pair_player_1_.CreateText(16, sf::Vector2f(1060, 55), sf::Text::Bold, "Jogador 1");
	pair_player_1_points_.CreateText(16, sf::Vector2f(1100, 78), sf::Text::Bold, "0");

	pair_player_2_.CreateText(16, sf::Vector2f(1180, 55), sf::Text::Bold, "Jogador 2");
	pair_player_2_points_.CreateText(16, sf::Vector2f(1220, 78), sf::Text::Bold, "0");

	truco_request_button_.CreateText(24, sf::Vector2f(1120, 680), 0, "Pedir truco!");

	accept_truco_request_.CreateText(20, sf::Vector2f(1100, 610), 0, "Aceitar truco");
	reject_truco_request_.CreateText(20, sf::Vector2f(1100, 640), 0, "Rejeitar truco");
	enraise_truco_request_.CreateText(20, sf::Vector2f(1100, 670), 0, "Aumentar truco");

}

void GameView::DisplayGameStatus(const GameStatus& status) {
	if (status.GameEnded) window_.close();

	if (status.NextPlayer == Characters::PLAYER_1) {
		current_player_value_.SetString("Jogador 1");
	}
	else if (status.NextPlayer == Characters::PLAYER_2) {
		current_player_value_.SetString("Jogador 2");
	}
	auto handResult = game_controller_->HandResult_();

	first_hand_wins_value_.SetString(std::to_string(status.Player1HandScore));
	second_hand_wins_value_.SetString(std::to_string(status.Player2HandScore));

	current_hand_value_.SetString(std::to_string(status.HandValue));

	pair_player_1_points_.SetString(std::to_string(status.Player1Score));
	pair_player_2_points_.SetString(std::to_string(status.Player2Score));
}

void GameView::UpdateGameStatus_(const GameStatus& status) {
	turned_up = status.Turned;

	playable_cards_.clear();
	for (const auto& card : status.Hands.at(self_character_)) {
		playable_cards_.emplace_back(card);
	}

	played_cards_.clear();
	for (const auto& play : status.PlayedCards) {
		Card card = play.card;
		played_cards_.emplace_back(card);
	}

	RenderGameStatus_();
}

void GameView::RenderGameStatus_() {
	turned_up.SetCardXYPosition(448, 250);
	turned_up.figure.setRotation(20);

	for (int i = 0; i < playable_cards_.size(); i++) {
		int offset = (CARD_WIDTH + 50) * i;
		playable_cards_[i].SetCardXYPosition(521 + offset, 650);
	}

	for (int i = 0; i < played_cards_.size(); i++) {
		int offset = (CARD_WIDTH + 50) * i;
		played_cards_[i].SetCardXYPosition(521 + offset, 400);
	}

	window_.requestFocus();
}

void GameView::Loop() {
	if (game_status_dirt) {
		auto status = game_controller_->GetGameStatus();
		UpdateGameStatus_(status);
		DisplayGameStatus(status);
		game_status_dirt = false;
	}
}

bool GameView::IsTrucoRequested_() {
	return game_controller_->GetGameStatus().TrucoRequester.has_value();
}

void GameView::EventLoop(const sf::Event& event) {
	WindowView::EventLoop(event);

	window_.clear();
	window_.draw(new_game_background_rect_);

	switch (event.type)
	{
	case sf::Event::MouseButtonReleased:
		for (int i = 0; i < playable_cards_.size(); i++) {

			if (playable_cards_[i].was_played) {
				continue;
			}

			bool canPlayCard = drop_area_.getGlobalBounds().intersects(playable_cards_[i].figure.getGlobalBounds());
			bool canPlayCoveredCard = playable_cards_[i].isCovered
				&& DeckOfCards.deck_rect.getGlobalBounds().intersects(playable_cards_[i].figure.getGlobalBounds());

			//play droped card
			if (canPlayCoveredCard || canPlayCard) {

				//if player plays with truco requested, the truco will be was accepted
				if (IsTrucoRequested_() 
					&& game_controller_->GetGameStatus().HandValue < 12) {
					game_controller_->AnswerTruco(self_character_, true);
				}

				playable_cards_[i].was_played = true;
				selected_card_ = playable_cards_[i];

				//commit the played card
				game_controller_->PlayCard(self_character_, i);

				drop_area_.setOutlineColor(sf::Color(255, 237, 217, 100));
			}
			if (playable_cards_[i].is_selected) {
				playable_cards_[i].is_selected = false;
				selected_card_ = turned_up;
				//drop area normal color
				drop_area_.setOutlineColor(sf::Color(255, 237, 217, 100));
			}
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			//truco handler.
			CheckTrucoActions_();

			for (auto& card : playable_cards_) {
				if (card.was_played) {
					continue;
				}

				if (card.figure.getGlobalBounds().contains(mouse_coord)) {
					card.is_selected = true;

					//drop area highlight
					drop_area_.setOutlineColor(sf::Color(36, 0, 0, 100));
				}
			}
			break;
			//drop area color change
	case sf::Event::MouseMoved:

		truco_request_button_.TextHover(mouse_coord);
		accept_truco_request_.TextHover(mouse_coord);
		reject_truco_request_.TextHover(mouse_coord);
		enraise_truco_request_.TextHover(mouse_coord);

		//just allow drag if was player turn
		bool isPlayerTurn = game_controller_->GetGameStatus().NextPlayer == self_character_;
		if (isPlayerTurn
			&& sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			for (auto& card : playable_cards_) {

				if (card.was_played) {
					continue;
				}

				//move selected card
				if (card.is_selected
					&& card.figure.getGlobalBounds().contains(mouse_coord)) {

					card.figure.setPosition(static_cast<float>(pos_mouse.x), static_cast<float>(pos_mouse.y));

					bool isFirstHand = game_controller_->GetGameStatus().Player1HandScore +
						game_controller_->GetGameStatus().Player2HandScore == 0;

					//change the card image to implements cover action
					if (!card.isCovered
						&& !isFirstHand
						&& DeckOfCards.deck_rect.getGlobalBounds().intersects(card.figure.getGlobalBounds())) {

						card.isCovered = true;
						card.SetCardXYTextureIndex(-1, -1);
					}
					if (card.isCovered
						&& !DeckOfCards.deck_rect.getGlobalBounds().intersects(card.figure.getGlobalBounds())) {
						card.isCovered = false;
						card.SetCardXYTextureIndex(card.Value, (double)card.Suit);
					}
				}

				//drop area highlight
				if (drop_area_.getGlobalBounds().intersects(selected_card_.figure.getGlobalBounds())) {
					drop_area_.setOutlineColor(sf::Color(36, 0, 0, 100));
				}
				else if (!drop_area_.getGlobalBounds().intersects(selected_card_.figure.getGlobalBounds())) {
					//drop area normal color
					drop_area_.setOutlineColor(sf::Color(255, 237, 217, 100));
				}
			}
		}
		break;
		}
	}

	std::lock_guard<std::mutex> lock(draw_mutex_);
	try
	{

		window_.draw(current_hand_text_.GetText());
		window_.draw(current_hand_value_.GetText());

		window_.draw(drop_area_);

		window_.draw(turned_up.figure);
		window_.draw(DeckOfCards.deck_rect);
		
		window_.draw(current_player_screen_value.GetText());
		window_.draw(current_player_text_.GetText());
		window_.draw(current_player_value_.GetText());

		window_.draw(first_hand_wins_.GetText());
		window_.draw(first_hand_wins_value_.GetText());
		window_.draw(second_hand_wins_.GetText());
		window_.draw(second_hand_wins_value_.GetText());

		window_.draw(score_text_.GetText());
		window_.draw(pair_player_1_.GetText());
		window_.draw(pair_player_1_points_.GetText());
		window_.draw(pair_player_2_.GetText());
		window_.draw(pair_player_2_points_.GetText());

		//just allow request truco in player turn
		if (!IsTrucoRequested_()
			&& game_controller_->GetGameStatus().HandValue <= 9
			&& game_controller_->GetGameStatus().NextPlayer == self_character_) {

			window_.draw(truco_request_button_.GetText());
		}
		else if (IsTrucoRequested_()
			&& game_controller_->GetGameStatus().TrucoRequester != self_character_
			&& game_controller_->GetGameStatus().HandValue < 12) {

			window_.draw(accept_truco_request_.GetText());
			window_.draw(reject_truco_request_.GetText());
			window_.draw(enraise_truco_request_.GetText());
		}

		//draw player cards
		for (auto& card : playable_cards_) {
			window_.draw(card.figure);
		}

		for (auto& card : played_cards_) {
			window_.draw(card.figure);
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << "Error during window draws";
		// log exception
		std::cerr << ex.what();
	}

	window_.display();

}

void GameView::CheckTrucoActions_()
{
	if (!IsTrucoRequested_()
		&& truco_request_button_.isTextSelected()) {
		game_controller_->RequestTruco(self_character_);
	}

	if (IsTrucoRequested_()) {
		if (accept_truco_request_.isTextSelected()) {
			game_controller_->AnswerTruco(self_character_, true);
		}

		if (reject_truco_request_.isTextSelected()) {
			game_controller_->AnswerTruco(self_character_, false);
		}

		if (enraise_truco_request_.isTextSelected()) {
			game_controller_->RequestTruco(self_character_);
		}
	}
}
