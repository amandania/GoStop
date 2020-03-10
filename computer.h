/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/

#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>
#include <iostream>
#include "player.h"

using namespace std;

class computer : public player
{

public:
	computer();
	~computer();
	card handle_card_choice();
	card choose_which_stack(vector<card> stack_found);
	void set_game_layout(vector<card> *layout);

protected:
	//Pointer to a vector of cards, which gets added too/removed from.
	vector<card> *game_layout;

};

#endif
