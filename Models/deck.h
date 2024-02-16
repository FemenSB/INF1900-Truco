#pragma once

#include "card.h"
#include <vector>

class Deck {
private:
	std::vector<Card> cards_;
	void Init_();
	void Shuffle_();
public:
	Deck();
	Card Pop();
};
