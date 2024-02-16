#pragma once

#include "../Models/game_status.h"
#include "game_controller_interface.h"
#include "cpu_partner_controller.h"
#include <memory>

class GameStatusObserver;

enum class RoundHandResult {
	NOT_ENDED,
	DRAW,
	PLAYER_1_WINS,
	PLAYER_2_WINS,
};

class GameController: public GameControllerInterface {
private:
	GameStatus current_status_;
	Characters next_hand_first_player_;

	std::vector<GameStatusObserver*> observers_;
	std::vector<GameStatusDirtObserver*> dirt_observers_;

	CpuPartnerController cpu_1_;
	CpuPartnerController cpu_2_;

	GameStatus NewGameStatus_();

	void AddPlayedCard_(Characters player, int card_index);
	void RemoveCardFromHand_(Characters player, int card_index);
	void AddTrucoToHandValue_();

	void NotifyObservers_();

	void UpdateStatus_();
	RoundHandResult RoundWinnerResult_() const;
	bool RoundWinnerPlay_(PlayedCard& out) const;
	void UpdateRound_(RoundHandResult result);

	RoundHandResult RoundResult_() const;
	void UpdateHand_(RoundHandResult result);

	void StartNewHand_();

	int EffectiveValue_(const Card& card) const;
	bool IsManilha_(const Card& card) const;
	int ManilhaValue_(SuitsEnum suit) const;

public:
	RoundHandResult HandResult_() const;
	GameController();
	void PlayCard(Characters player, int card_index) override;
	void RequestTruco(Characters requester) override;
	void AnswerTruco(Characters requester, bool accept) override;
	void AddStatusObserver(GameStatusObserver* observer) override;
	void RemoveStatusObserver(GameStatusObserver* observer) override;
	void AddStatusDirtObserver(GameStatusDirtObserver* observer) override;
	void RemoveStatusDirtObserver(GameStatusDirtObserver* observer) override;
	GameStatus GetGameStatus() override;
	Characters Winner() override;
};
