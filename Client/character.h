#pragma once
#include <iostream>
#include <string>
#include <vector>

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>

#include "assets.h"
#include "card_view.h"

using namespace std;

class Character {
	
	string name_;
	int score_;

	vector<CardView> hand_;

public:
	Character();
	~Character();
};