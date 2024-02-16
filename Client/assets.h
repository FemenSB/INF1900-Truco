#include <SFML\Graphics.hpp>

#include <iostream>
#include <vector>
#include <fstream>

#pragma once

class AssetsManager {
	AssetsManager();
	~AssetsManager();
	static AssetsManager* instance_;
	std::map <std::string, sf::Texture> textures_;
	sf::Font main_font_;	
	bool is_loaded_ = false;
	void Load_();

public:
	static AssetsManager* GetInstance();
	sf::Font* font();
	sf::Texture& operator[](std::string);	
};