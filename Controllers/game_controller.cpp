#include "game_controller.h"
#include "game_status_observer.h"
#include "../Models/constants.h"
#include "../Models/deck.h"
#include <stdexcept>
#include <algorithm>

namespace {
	Hand GetHand(Deck& deck) {
		Hand result;
		for (int i = 0; i < 3; i++) {
			result.push_back(deck.Pop());
		}
		return result;
	}

	GameStatus DealCards(Deck &deck) {
		Card turned = deck.Pop();
		GameStatus result{ turned };
		for (int i = 0; i <= (int)Characters::LAST; i++) {
			result.Hands[(Characters)i] = GetHand(deck);
		}
		return result;
	}
	/// <summary>
	/// Set the cards to the players and the turned
	/// </summary>
	/// <param name="deck"></param>
	/// <param name="status"></param>
	void DealCards(Deck& deck, GameStatus& status) {		
		Card turned = deck.Pop();
		status.Turned = turned;
		for (int i = 0; i <= (int)Characters::LAST; i++) {
			status.Hands[(Characters)i] = GetHand(deck);
		}
	}

	Pairs Pair(Characters character) {
		if (character == Characters::PLAYER_1_CPU_PARTNER
				|| character == Characters::PLAYER_1) {
			return Pairs::PLAYER_1_PAIR;
		}
		return Pairs::PLAYER_2_PAIR;
	}

	Characters PlayerNextTo(Characters player) {
		const int player_index = (int)player;
		const int last_index = (int)Characters::LAST;
		return (Characters)((player_index + 1) % (last_index + 1));
	}

	PlayedCard DummyPlayedCard() {
		return { Card(SuitsEnum::CLUBS, 1), Characters::PLAYER_1, Pairs::PLAYER_1_PAIR };
	}

	RoundHandResult CharacterLostRoundResult(Characters character) {
		switch (character) {
		case Characters::PLAYER_1:
			return RoundHandResult::PLAYER_2_WINS;
		case Characters::PLAYER_2:
			return RoundHandResult::PLAYER_1_WINS;
		}
		throw std::exception("No viable conversion");
	}
}

GameController::GameController()
		: current_status_(NewGameStatus_()),
		cpu_1_(this, Characters::PLAYER_1_CPU_PARTNER),
		cpu_2_(this, Characters::PLAYER_2_CPU_PARTNER) {
	next_hand_first_player_ = Characters::PLAYER_1;
}

GameStatus GameController::NewGameStatus_() {
	Deck deck;
	GameStatus result = DealCards(deck);
	result.GameEnded = false;
	result.Player1Score = 0;
	result.Player2Score = 0;
	result.Player1HandScore = 0;
	result.Player2HandScore = 0;
	result.HandValue = 1;
	result.TrucoRequester = std::nullopt;
	
	//Player 1 starts playng the card
	result.NextPlayer = Characters::PLAYER_1;

	return result;
}

void GameController::PlayCard(Characters player, int card_index) {
	if (player != current_status_.NextPlayer) return;
	if (current_status_.TrucoRequester.has_value()) return; // Can't play cards until truco is accepted/denied
	AddPlayedCard_(player, card_index);
	
	RemoveCardFromHand_(player, card_index);
	
	UpdateStatus_();
}

void GameController::AddPlayedCard_(Characters player, int card_index) {
	const Card card = current_status_.Hands[player][card_index];
	PlayedCard played{ card, player, Pair(player) };
	current_status_.PlayedCards.push_back(played);
}

void GameController::RemoveCardFromHand_(Characters player, int card_index) {
	auto card_it = current_status_.Hands[player].begin() + card_index;
	current_status_.Hands[player].erase(card_it);
}

void GameController::RequestTruco(Characters requester) {
	if (current_status_.TrucoRequester.has_value()) {
		if (current_status_.TrucoRequester.value() == requester) {
			return;
		}
		// If truco is already requested, a new truco request accepts the current truco
		AddTrucoToHandValue_();
	}
	current_status_.TrucoRequester = std::make_optional(requester);

	NotifyObservers_();
}

void GameController::AnswerTruco(Characters requester, bool accept) {
	if (!current_status_.TrucoRequester.has_value()) return;
	if (current_status_.TrucoRequester.value() == requester) return;
	if (accept) {
		current_status_.TrucoRequester = std::nullopt;
		AddTrucoToHandValue_();
	}
	else {
		auto round_result = CharacterLostRoundResult(requester);
		UpdateHand_(round_result);
	}
	NotifyObservers_();
}

void GameController::AddTrucoToHandValue_() {
	if (current_status_.HandValue == 1) {
		current_status_.HandValue = 3;
	}
	else {
		current_status_.HandValue += 3;
	}
}

void GameController::NotifyObservers_() {
	auto status = GetGameStatus();
	for (auto observer : observers_) {
		observer->GameStatusChanged(status);
	}
	for (auto observer : dirt_observers_) {
		observer->game_status_dirt = true;
	}
}

void GameController::UpdateStatus_() {
	RoundHandResult round_result = RoundResult_();
	UpdateRound_(round_result);
	RoundHandResult hand_result = HandResult_();
	UpdateHand_(hand_result);
	NotifyObservers_();
}

RoundHandResult GameController::RoundResult_() const {
	if (current_status_.PlayedCards.size() < 4) return RoundHandResult::NOT_ENDED;
	return RoundWinnerResult_();
}

RoundHandResult GameController::RoundWinnerResult_() const {
	PlayedCard winning_play = DummyPlayedCard();
	if (RoundWinnerPlay_(winning_play)) {
		if (winning_play.player_pair == Pairs::PLAYER_1_PAIR)
			return RoundHandResult::PLAYER_1_WINS;
		return RoundHandResult::PLAYER_2_WINS;
	}
	return RoundHandResult::DRAW;
}

bool GameController::RoundWinnerPlay_(PlayedCard& out) const {
	bool draw = true;
	Pairs winning_pair = Pairs::PLAYER_1_PAIR;
	int winning_value = -1;
	PlayedCard winning_play = DummyPlayedCard();
	for (int i = 0; i < current_status_.PlayedCards.size(); i++) {
		const auto play = current_status_.PlayedCards[i];
		const int play_value = EffectiveValue_(play.card);
		if (play_value > winning_value) {
			winning_value = play_value;
			winning_pair = play.player_pair;
			winning_play = play;
			draw = false;
		}
		if (play_value == winning_value && play.player_pair != winning_pair) {
			draw = true;
		}
	}
	if (draw) return false;
	out = winning_play;
	return true;
}

void GameController::UpdateRound_(RoundHandResult result) {
	if (result == RoundHandResult::NOT_ENDED) {
		current_status_.NextPlayer = PlayerNextTo(current_status_.NextPlayer);
		return;
	}
	if (result == RoundHandResult::PLAYER_1_WINS || result == RoundHandResult::DRAW) {
		current_status_.Player1HandScore++;
	}
	if (result == RoundHandResult::PLAYER_2_WINS || result == RoundHandResult::DRAW) {
		current_status_.Player2HandScore++;
	}

	PlayedCard winning_play = DummyPlayedCard();
	if (RoundWinnerPlay_(winning_play)) {
		current_status_.NextPlayer = winning_play.player;
	}

	current_status_.PlayedCards.clear();
}

RoundHandResult GameController::HandResult_() const {

	if (current_status_.Player1HandScore == 2
		&& current_status_.Player2HandScore < 2) {
		return RoundHandResult::PLAYER_1_WINS;
	}
	if (current_status_.Player1HandScore < 2
		&& current_status_.Player2HandScore == 2) {
		return RoundHandResult::PLAYER_2_WINS;
	}
	if (current_status_.Player1HandScore == 3
		&& current_status_.Player2HandScore == 3) {
		return RoundHandResult::DRAW;
	}
	if (current_status_.Player1HandScore == 3) {
		return RoundHandResult::PLAYER_1_WINS;
	}
	if (current_status_.Player2HandScore == 3) {
		return RoundHandResult::PLAYER_2_WINS;
	}
	return RoundHandResult::NOT_ENDED;
}

void GameController::UpdateHand_(RoundHandResult result) {
	if (result == RoundHandResult::PLAYER_1_WINS) {
		current_status_.Player1Score += current_status_.HandValue;
	} else if (result == RoundHandResult::PLAYER_2_WINS) {
		current_status_.Player2Score += current_status_.HandValue;
	}
	if (result != RoundHandResult::NOT_ENDED) {
		StartNewHand_();
	}
}

void GameController::StartNewHand_() {
	if (current_status_.Player1Score >= 12 || current_status_.Player2Score >= 12) {
		current_status_.GameEnded = true;
		return;
	}
	Deck deck;
	DealCards(deck, current_status_);
	
	current_status_.PlayedCards.clear();
	current_status_.Player1HandScore = 0;
	current_status_.Player2HandScore = 0;
	current_status_.HandValue = 1;
	current_status_.NextPlayer = next_hand_first_player_;
	current_status_.TrucoRequester = std::nullopt;
	next_hand_first_player_ = PlayerNextTo(next_hand_first_player_);
}

int GameController::EffectiveValue_(const Card& card) const {
	if (card.isCovered) return 0;

	if (IsManilha_(card)) {
		return ManilhaValue_(card.Suit);
	}
	if (card.Value < 3) { // Cards A, 2 and 3 are stronger than the others
		return CARD_KING_VALUE + card.Value + 1;
	}
	return card.Value;
}

bool GameController::IsManilha_(const Card& card) const {
	const int manilha_value = (current_status_.Turned.Value + 1) % (CARD_KING_VALUE + 1);
	return card.Value == manilha_value;
}

int GameController::ManilhaValue_(SuitsEnum suit) const {
	// 100 is an arbitrary value supposed to be greater than the value of any card
	return 100 + (int)suit;
}

void GameController::AddStatusObserver(GameStatusObserver* observer) {
	observers_.push_back(observer);
	observer->GameStatusChanged(GetGameStatus());
}

void GameController::RemoveStatusObserver(GameStatusObserver* observer) {
	observers_.erase(std::find(observers_.begin(), observers_.end(), observer));
}

void GameController::AddStatusDirtObserver(GameStatusDirtObserver* observer) {
	dirt_observers_.push_back(observer);
	observer->game_status_dirt = true;
}

void GameController::RemoveStatusDirtObserver(GameStatusDirtObserver* observer) {
	dirt_observers_.erase(std::find(dirt_observers_.begin(), dirt_observers_.end(), observer));
}

GameStatus GameController::GetGameStatus() {
	return current_status_;
}

Characters GameController::Winner() {
	return current_status_.Player1Score > current_status_.Player2Score ?
		Characters::PLAYER_1 : Characters::PLAYER_2;
}
