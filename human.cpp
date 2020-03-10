/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/

#include "human.h"

human::human() {
	set_name("Human");
}

card human::handle_card_choice() {
	vector<card> human_hand = *this->get_hand();
	int pos = -1;
	cout << "Which card would you like to choose: (card_pos): ";
	cin >> pos;
	int size = human_hand.size();

	while (!cin || (pos < 0 || pos >= size) ) {


		cout << "Which card would you like to choose: (card_pos): ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> pos;
	}
	
	card toreturn = human_hand[pos];

	if (pos == this->get_hand()->size()-1) {
		this->get_hand()->pop_back();
	}
	else {
		*this->get_hand()->erase(this->get_hand()->begin() + pos);
	}
	return toreturn;
}

card human::choose_which_stack(vector<card> stack_found) {
	cout << "Which card would you like to stack with? ";

	int i = 0;
	for (card cardtype : stack_found) {
		cout << "" << cardtype.get_name() << "(" << i << "), ";
		i++;
	}


	int pos = -1;
	cin >> pos;
	int size = stack_found.size();

	while (!cin || (pos < 0 || pos > size)) {
		cout << "Which card would you like to stack with? ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> pos;
	}
	card toreturn = stack_found[pos];

	return toreturn;

}

human::~human() {

}