#pragma once

#include <vector>

#define CARD_QUEEN_VALUE 7
#define CARD_PRINCE_VALUE 8
#define CARD_KING_VALUE 9

enum class SuitsEnum { // Ordered from weakest to strongest
	DIMOND = 0,
	SPADES = 1,
	HEARTS = 2,
	CLUBS = 3,
	LAST = 3,
};

enum class Characters { // In play order
	PLAYER_1 = 0,
	PLAYER_2_CPU_PARTNER = 1,
	PLAYER_1_CPU_PARTNER = 2,
	PLAYER_2 = 3,
	LAST = 3,
};

enum class Pairs {
	PLAYER_1_PAIR,
	PLAYER_2_PAIR,
};
