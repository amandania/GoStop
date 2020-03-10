/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/
/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/


#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

class player
{
public:
	player();
	player(string name);
	virtual ~player();
	virtual card handle_card_choice();
	int handle_pre_turn_options();
	
	virtual card choose_which_stack(vector<card> stack_found);

	int get_help_move(vector<card> layout);

	string get_name();
	void set_name(string name);
	vector<card> *get_hand();
	vector<card> *get_capture_pile();
	void print_hand(bool in_option_menu);
	void print_capture_pile();
	bool is_playing();
	void set_is_playing(bool playing);
	int get_score();
	void generate_score();

private:

	//Hold all player data. 
	string name;
	vector<card> *hand;
	vector<card> *capture_pile;


	//Keep track of current players score
	int score;
};

#endif
