#include <SFML/Graphics.hpp>
#include "assets.h"

class Text
{
private:
	AssetsManager* asset_manager_;


public:	
	void CreateText(sf::Text& text, std::string string, int fontSize, sf::Color color, sf::Text::Style style, sf::Vector2f position);

	Text();
	~Text() = default;
};
