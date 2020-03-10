/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/

#include "computer.h"

computer::computer() {
	set_name("Computer");
}



card computer::handle_card_choice() {
	card toreturn;
	vector<card> hand = *this->get_hand();
	int hand_slot = get_help_move(*this->game_layout);

	toreturn = hand[hand_slot];

	if (hand_slot == this->get_hand()->size() - 1) {
		this->get_hand()->pop_back();
	}
	else {
		*this->get_hand()->erase(this->get_hand()->begin() + hand_slot);
	}
	return toreturn;
}
card computer::choose_which_stack(vector<card> stack_found) {
	
	return stack_found[0];;
}

void computer::set_game_layout(vector<card> *layout) {
	this->game_layout = layout;
}

computer::~computer() {

}