#include "deck.h"
#include <random>

Deck::Deck() {
	Init_();
	Shuffle_();
}

void Deck::Init_() {
	//foreach suit
	for (int i = 0; i <= (int)SuitsEnum::LAST; i++)
	{
		SuitsEnum eCurrent = (SuitsEnum)i;

		//add suit number cards
		for (int value = 0; value < CARD_QUEEN_VALUE; value++)
		{
			Card card(eCurrent, value);
			cards_.push_back(card);
		}

		Card queen(eCurrent, CARD_QUEEN_VALUE);
		cards_.push_back(queen);

		Card prince(eCurrent, CARD_PRINCE_VALUE);
		cards_.push_back(prince);

		Card king(eCurrent, CARD_KING_VALUE);
		cards_.push_back(king);
	}
}

void Deck::Shuffle_() {

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(cards_.begin(), cards_.end(), g);
}

Card Deck::Pop() {
	Card result = cards_.back();
	cards_.pop_back();
	return result;
}
