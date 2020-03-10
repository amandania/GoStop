/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/

#pragma once
#ifndef CARD_H
#define CARD_H

#include <algorithm>
#include <string>
#include <vector>
using namespace std;

enum CARD_VALUE {
    Ace = 1, Two, Three, Four,
    Five, Six, Seven, Eight,
    Nine, Ten, Jack, Queen, King
};

enum CARD_SUITE {
    Spade, Heart, Diamond, Club
};
class card
{

public:
    card();
    card(CARD_VALUE val, CARD_SUITE suite);
    card(CARD_VALUE val, CARD_SUITE suite, bool face_up);
    ~card();
    bool is_facing_up();
    void set_face_up(bool face_up);


	int get_value();
	CARD_SUITE get_suite();

	int get_slot_position();
	void set_slot_position(int slot);

	vector<card> *get_stack_children();
	
    string get_name();

private:
	CARD_VALUE value;
	CARD_SUITE suite;
	bool is_face_up;
	int stack_amount;
	vector<card> stack_children;
	int slot_position;
};


#endif