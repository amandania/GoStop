#pragma once
#ifndef GAMEROUND_H
#define GAMEROUND_H

#include "human.h"
#include "computer.h"
#include "deck.h"
#include <algorithm>
#include <vector>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>
#include <iterator>
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <map>


namespace fs = filesystem;
using namespace std;


enum PileType {
	Hand, Layout
};

class gameround
{

public:
	gameround();
	gameround(bool in_debug, human *human_player, computer *computer_player);
	~gameround();

	void start_round(bool is_loaded);

	void deal_from_deck(vector<card> *to_pile, const int amount, PileType pile_type);
	void deal_startup();
	void add_cards_to_hand(vector<card> cards, vector<card> *hand);
	void sort_hand(vector<card> *hand, PileType pile_type);
	void add_card_to_layout(player *player, card card_to_add);
	void add_cards_to_layout(vector<card> cards);
	void add_to_capture_pile(vector<card> capture_cards, player *player);
	void print_line_break(int option);
	card get_highest_child_card();
	void print_layout();
	void save_game();
	void load_game();
	vector<string> split_string(const string &s, char delim);
	vector<card> parse_cards(vector<string> cards);
	player *decide_first_player();
	void end_round();

private:
	bool in_debug;
	human *human_player;
	computer *computer_player;
	bool is_playing;

	//A round deck which gets drawn from at different points of the game.
	deck *round_deck;

	//Our game board layout
	vector<card> layout;
	//Round count track
	int round_count;
	//Hold a pointer of type player, as our current player
	player *current_player;
};


#endif