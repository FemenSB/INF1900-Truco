#include "assets.h"

AssetsManager* AssetsManager::instance_ = nullptr;

AssetsManager::AssetsManager() {
	textures_.clear();
}

AssetsManager::~AssetsManager() = default;

AssetsManager* AssetsManager::GetInstance() {
	if (instance_ == nullptr) {
		instance_ = new AssetsManager();
		instance_->Load_();
	}
	return instance_;
}

void AssetsManager::Load_() {
	if (!is_loaded_) {
		textures_["none"].loadFromFile("./resources/imgs/none.png");
		textures_["none"].setSmooth(true);

		textures_["cards"].loadFromFile("./resources/imgs/cards.png");
		textures_["cards"].setSmooth(true);

		textures_["mainPage"].loadFromFile("./resources/imgs/mainPage.png");
		textures_["mainPage"].setSmooth(true);

		textures_["backCard"].loadFromFile("./resources/imgs/backCard.png");
		textures_["backCard"].setSmooth(true);

		textures_["deck"].loadFromFile("./resources/imgs/deck.png");
		textures_["deck"].setSmooth(true);

		main_font_.loadFromFile("./resources/fonts/roboto.ttf");

		is_loaded_ = true;
	}
}


sf::Font* AssetsManager::font() {
	return &main_font_;
}

sf::Texture& AssetsManager::operator[](std::string data) {
	if (textures_.count(data) > 0) {
		return textures_[data];
	}
	else return textures_["none"];
}