#include "text.h"

Text::Text() : asset_manager_(AssetsManager::GetInstance()) {
}

void Text::CreateText(sf::Text& text, std::string string, int fontSize, sf::Color color = sf::Color::White, sf::Text::Style style, sf::Vector2f position) {
	text.setFont(*asset_manager_->font());

	text.setString(string);
	text.setCharacterSize(fontSize);
	text.setFillColor(color);
	text.setStyle(style);
	text.setPosition(position);
}
