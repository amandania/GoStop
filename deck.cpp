#include "deck.h"

deck::deck() {
	for (int i = Ace; i != King + 1; i++) {
		CARD_VALUE card_value = static_cast<CARD_VALUE>(i);
		for (int k = Ace; i != King + 1; k++) {
			CARD_VALUE card_value = static_cast<CARD_VALUE>(i);
			for (int j = Spade; j != Club + 1; j++) {
				CARD_SUITE card_suite = static_cast<CARD_SUITE>(j);
				card card_to_add = card(card_value, card_suite);
				add_card(card_to_add);
			}
		}
	}
}

deck::deck(int number_of_decks) {
	for (int deckcount = 0; deckcount < number_of_decks; deckcount++) {
		for (int i = Ace; i != King + 1; i++) {
			CARD_VALUE card_value = static_cast<CARD_VALUE>(i);
			for (int j = Spade; j != Club + 1; j++) {
				CARD_SUITE card_suite = static_cast<CARD_SUITE>(j);
				card card_to_add = card(card_value, card_suite, false);
				add_card(card_to_add);
			}
		}
	}
}

deck::~deck() {

}

card deck::draw() {
	card drawn_card = get_cards().front();
	this->cards_in_deck.erase(this->cards_in_deck.begin());
	return drawn_card;
}

vector<card> deck::draw(int amount) {
	vector<card> copy_first_amounts;
	for (int i = 0; i < amount; i++) {
		//anytime we draw, we draw and face it up.
		this->get_cards()[i].set_face_up(true);
		copy_first_amounts.push_back(this->get_cards()[i]);
	}
	this->cards_in_deck.erase(this->cards_in_deck.begin(), this->cards_in_deck.begin() + amount);
	return copy_first_amounts;
}

void deck::shuffle_cards() {
	//cout << " size of deck "<< this->cards_in_deck.size() << endl;
	random_device r;
	seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
	mt19937 eng(seed);
	
	shuffle(begin(this->cards_in_deck), end(this->cards_in_deck), eng);
}

void deck::print_deck() {
	for (card cardtype : get_cards()) {
		cout << "" << cardtype.get_name() << ", ";
	}
	cout << endl;
}

void deck::add_card(card to_add) {
	cards_in_deck.insert(cards_in_deck.end(), to_add);
}

void deck::clear_deck() {
	this->cards_in_deck.clear();
}

vector<card> deck::get_cards() {
	return this->cards_in_deck;
}