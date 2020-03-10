#pragma once

/*
Deck will be constrcuted with all 104 cards for the game

private:
	a queue of cards, to represent the entire active deck

public members include
shuffle
drawcard returns card 
print_deck


*/

#ifndef DECK_H
#define DECK_H
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <random>
#include "card.h"

using namespace std;


//This deck will also be treated as the "Stock"
class deck
{

public:
	deck();
	deck(int number_of_decks);
	~deck();
	void shuffle_cards();
	card draw();
	vector<card> draw(int amount);
	void print_deck();
	void add_card(card to_add);
	vector<card> get_cards();
	void clear_deck(); 

private:
	//List of active cards in deck.
	vector<card> cards_in_deck;
};


#endif