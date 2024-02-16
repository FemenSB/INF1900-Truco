#pragma once

#include "../Models/game_status.h"

class GameStatusObserver {
public:
	virtual void GameStatusChanged(const GameStatus& status) = 0;
};

class GameStatusDirtObserver {
public:
	bool game_status_dirt = false;
};
