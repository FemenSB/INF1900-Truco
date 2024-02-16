#pragma once

#include "game_controller_interface.h"
#include "game_status_observer.h"

class CpuPartnerController: public GameStatusObserver {
private:
	GameControllerInterface* controller_;
	Characters self_character_;
public:
	CpuPartnerController(GameControllerInterface* controller, Characters self_character);
	~CpuPartnerController();
	void GameStatusChanged(const GameStatus& status) override;
};
