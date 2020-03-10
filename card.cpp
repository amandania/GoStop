/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/

#include "card.h"
#include <iostream>

card::card() {
	this->value = static_cast<CARD_VALUE>(-1);
	this->suite = static_cast<CARD_SUITE>(-1);
}
card::card(CARD_VALUE val, CARD_SUITE suite) {
	this->value = val;
	this->suite = suite;
	set_face_up(true);
}
card::card(CARD_VALUE val, CARD_SUITE suite, bool face_up) {
	this->value = val;
	this->suite = suite;
	set_face_up(face_up);
}
card::~card() {

}

bool card::is_facing_up() {
	return this->is_face_up;
}
void card::set_face_up(bool face_up) {
	this->is_face_up = face_up;
}
int card::get_value() {
	return this->value;
}
int card::get_slot_position() {
	return this->slot_position;
}
void card::set_slot_position(int pos) {
	this->slot_position = pos;
}

CARD_SUITE card::get_suite() {
	return this->suite;
}

string card::get_name() {
	string name = "";
	switch (value) {
		case Ace:
			name = "A";
			break;
		case Two:
			name = "2";
			break;
		case Three:
			name = "3";
			break;
		case Four:
			name = "4";
			break;
		case Five:
			name = "5";
			break;
		case Six:
			name = "6";
			break;
		case Seven:
			name = "7";
			break;
		case Eight:
			name = "8";
			break;
		case Nine:
			name = "9";
			break;
		case Ten:
			name = "10";
			break;
		case Jack:
			name = "J";
			break;
		case Queen:
			name = "Q";
			break;
		case King:
			name = "K";
			break;
	}
	switch (suite) {
		case Heart:
			name += "H";
			break;
		case Spade:
			name += "S";
			break;
		case Club:
			name += "C";
			break;
		case Diamond:
			name += "D";
			break;
	}
	return name;
}

vector<card> *card::get_stack_children() {
	return &this->stack_children;
}