#include "card_view.h"

CardView::CardView(const Card& card)
	: CardView(card.Suit, card.Value) {}


CardView::CardView(SuitsEnum suit, int value)
		: Card(suit, value),
		asset_manager_(AssetsManager::GetInstance()) {
	cards_texture_size_ = asset_manager_->operator[]("cards").getSize();
	back_card_texture_size_ = asset_manager_->operator[]("backCard").getSize();
	
	cards_texture_size_.x /= MAX_CARD_X_TEXTURE_INDEX;
	cards_texture_size_.y /= MAX_CARD_Y_TEXTURE_INDEX;

	figure.setSize((sf::Vector2f(CARD_WIDTH, CARD_HEIGHT)));
	figure.setOrigin(CARD_WIDTH / 2, CARD_HEIGHT / 2);

	cards_uv_rect_.width = cards_texture_size_.x;
	cards_uv_rect_.height = cards_texture_size_.y;
	
	back_card_uv_rect_.width = back_card_texture_size_.x;
	back_card_uv_rect_.height = back_card_texture_size_.y;
	back_card_uv_rect_.left = 0; back_card_uv_rect_.top = 0;

	SetCardXYTextureIndex(Value, (double)Suit);
}

void CardView::SetCardXYTextureIndex(double valueX, double valueY) {
	if (valueX < 0 || valueY < 0) {
		figure.setTexture(&(asset_manager_->operator[]("backCard")));						
		figure.setTextureRect(back_card_uv_rect_);
	}
	else {
		figure.setTexture(&(asset_manager_->operator[]("cards")));

		if (valueX > MAX_CARD_X_TEXTURE_INDEX)
			valueX = MAX_CARD_X_TEXTURE_INDEX;

		if (valueY > MAX_CARD_Y_TEXTURE_INDEX)
			valueY = MAX_CARD_Y_TEXTURE_INDEX;

		card_texture_y_index_ = valueY;
		card_texture_x_index_ = valueX;

		cards_uv_rect_.left = card_texture_x_index_ * cards_uv_rect_.width;
		cards_uv_rect_.top = card_texture_y_index_ * cards_uv_rect_.height;

		figure.setTextureRect(cards_uv_rect_);
	}
}

void CardView::SetCardXYPosition(double valueX, double valueY) {
	card_texture_x_position = valueX;
	card_texture_y_position = valueY;
	figure.setPosition(valueX, valueY);
}

void CardView::CheckMoveCard(sf::Vector2i mousePosition, sf::Vector2f mouse_coord) {

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && figure.getGlobalBounds().contains(mouse_coord)) {
		//get the mouse position according the windown. Starting in 0, 0 in left top. 
		figure.setPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
	}
}
