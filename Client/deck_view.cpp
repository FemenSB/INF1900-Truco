#include "deck_view.h"

#include "card_view.h"

DeckView::DeckView() {

}

DeckView::~DeckView() = default;

void DeckView::Init() {
	deck_rect.setSize(sf::Vector2f(250.0f, 150.0f));
	deck_rect.setOrigin(deck_rect.getSize().x / 2, deck_rect.getSize().y / 2);
	deck_rect.setTexture(&(AssetsManager::GetInstance()->operator[]("deck")));
}

/// <summary>
/// finding the position of the card in the deck and remove it
/// </summary>
/// <param name_="card"></param>
//void Deck::RemoveCard(const CardView card) {
//	/*auto it = std::find(Cards.begin(), Cards.end(), card);
//	Cards.erase(it);*/
//}