#include "text.h"

Text::Text() : asset_manager_(AssetsManager::GetInstance()) {
}

void Text::CreateText(int fontSize, sf::Vector2f position, int style, std::string string, sf::Color color) {
	text_.setFont(*asset_manager_->font());

	text_.setString(string);
	text_.setCharacterSize(fontSize);
	text_.setFillColor(color);
	text_.setStyle(style);
	text_.setPosition(position);
}

void Text::SetString(std::string value) {
	text_.setString(value);
}

void Text::SetOutlineThickness(const int value) {
	text_.setOutlineThickness(value);
}

void Text::TextHover(sf::Vector2f mouse_coord) {
	if (text_.getGlobalBounds().contains(mouse_coord)) {
		text_.setOutlineThickness(3);
	}
	else if (text_.getOutlineThickness() != 0) {
		text_.setOutlineThickness(0);
	}
}

bool Text::isTextSelected()
{
	return text_.getOutlineThickness() != 0;
}

sf::Text Text::GetText() {
	return text_;
}