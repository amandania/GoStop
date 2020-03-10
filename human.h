/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/

#ifndef HUMAN_H
#define HUMAN_H

#include <vector>
#include <iostream>
#include "player.h"

using namespace std;

class human : public player
{

public:
	human();
	~human();
	card handle_card_choice();
	card choose_which_stack(vector<card> stack_found);

};

#endif
