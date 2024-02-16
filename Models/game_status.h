#pragma once

#include "card.h"
#include <unordered_map>
#include <optional>

using Hand = std::vector<Card>;

struct PlayedCard {
	Card card;
	Characters player;
	Pairs player_pair;
};

struct GameStatus {
	Card Turned;

	std::unordered_map<Characters, Hand> Hands;

	int Player1Score;
	int Player2Score;

	int HandValue; // starts at 1, may increase to 3, 6, 9 and 12
	int Player1HandScore;
	int Player2HandScore;
	std::optional<Characters> TrucoRequester;

	std::vector<PlayedCard> PlayedCards;
	Characters NextPlayer;

	bool GameEnded;
};
