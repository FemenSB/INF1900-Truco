#pragma once

#include <SFML/Graphics.hpp>
#include "assets.h"
#include "../Models/card.h"

#define CARD_WIDTH 90
#define CARD_HEIGHT 120

#define MAX_CARD_X_TEXTURE_INDEX 10
#define MAX_CARD_Y_TEXTURE_INDEX 4

class CardView : public Card
{
private:
	AssetsManager* asset_manager_;
	int card_texture_x_index_ = -1;
	int card_texture_y_index_ = -1;

	sf::IntRect cards_uv_rect_;
	sf::Vector2u cards_texture_size_;

	sf::IntRect back_card_uv_rect_;
	sf::Vector2u back_card_texture_size_;

public:	
	float card_texture_x_position = 0;
	float card_texture_y_position = 0;

	sf::RectangleShape figure;
	bool was_played = false;
	bool is_selected = false;	
	bool is_turned_up = false;

	CardView(const Card& card);
	CardView(SuitsEnum nipe, int value);
	~CardView() = default;

	void SetCardXYPosition(double value_x, double value_y);	
	void SetCardXYTextureIndex(double value_x, double value_y);	

	void CheckMoveCard(sf::Vector2i mouse_position, sf::Vector2f mouse_coord);
};
