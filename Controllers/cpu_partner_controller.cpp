#include "cpu_partner_controller.h"

CpuPartnerController::CpuPartnerController(GameControllerInterface* controller,
		Characters self_character)
		: controller_(controller),
		self_character_(self_character) {
	controller_->AddStatusObserver(this);
}

CpuPartnerController::~CpuPartnerController() {
	controller_->RemoveStatusObserver(this);
}

void CpuPartnerController::GameStatusChanged(const GameStatus& status) {	
	if (status.NextPlayer == self_character_
			&& status.Hands.at(self_character_).size() > 0) {
		controller_->PlayCard(self_character_, 0);
	}
}
