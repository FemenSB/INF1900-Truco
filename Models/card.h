#pragma once

#include "constants.h"

class Card
{
public:
	SuitsEnum Suit;
	int Value;
	bool isCovered = false;

	Card(SuitsEnum nipe, int value);
	~Card() = default;
};
