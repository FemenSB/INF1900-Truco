#pragma once
#include <SFML/Graphics.hpp>

class CardView;

using namespace std;

class DeckView
{

public:
	sf::RectangleShape deck_rect;

	DeckView();
	~DeckView();
	void Init();
	//void RemoveCard(CardView card);
};

