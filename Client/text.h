#include <SFML/Graphics.hpp>
#include "assets.h"

class Text
{
private:
	AssetsManager* asset_manager_;
	sf::Text text_;

public:
	void CreateText(int font_size, sf::Vector2f position, int style = 0 , std::string string = "", sf::Color color = sf::Color::White);

	sf::Text GetText();
	void SetString(std::string);
	void SetOutlineThickness(const int value);
	void TextHover(sf::Vector2f mouse_coord);
	bool isTextSelected();

	Text();
	~Text() = default;
};
