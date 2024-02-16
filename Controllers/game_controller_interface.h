#pragma once

#include "../Models/game_status.h"
#include "game_status_observer.h"

class GameControllerInterface {
public:
	virtual void PlayCard(Characters player, int card_index) = 0;
	virtual void RequestTruco(Characters requester) = 0;
	virtual void AnswerTruco(Characters requester, bool accept) = 0;
	virtual void AddStatusObserver(GameStatusObserver* observer) = 0;
	virtual void RemoveStatusObserver(GameStatusObserver* observer) = 0;
	virtual void AddStatusDirtObserver(GameStatusDirtObserver* observer) = 0;
	virtual void RemoveStatusDirtObserver(GameStatusDirtObserver* observer) = 0;
	virtual GameStatus GetGameStatus() = 0;
	virtual Characters Winner() = 0;
};

