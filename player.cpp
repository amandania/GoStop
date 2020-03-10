/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/

#include "player.h";
#include <iostream>

player::player() {
	this->hand = new vector<card>();
	this->capture_pile = new vector<card>();
	this->score = 0;
}

player::player(string name) {
	this->name = name;
	this->hand = new vector<card>();
	this->capture_pile = new vector<card>();
	this->score = 0;
}

/* *********************************************************************
Function Name: handle_card_choice
Purpose: To return a card from players hand
Parameters: None gets overloaded
Return Value: Chosen card from hand
Local Variables:
			temp, a temporary card used to return hand card
********************************************************************* */
card player::handle_card_choice() {
	card temp;
	return temp;
}


/* *********************************************************************
Function Name: choose_which_stack
Purpose: To return a card from players found pairs
Parameters: vector<card> stack, will tusually only have size of 2, in which player chooses.
Return Value: Chosen card from stack_found
Local Variables:
			to_return, a temporary card used to return hand card
********************************************************************* */
card player::choose_which_stack(vector<card> stack_found) {
	card to_return;

	return to_return;
}


/* *********************************************************************
Function Name: handle_pre_turn_options
Purpose: To offer options before each turn
Return Value: int, for the option choice selected
Local Variables:
			pos, to track the cin recivied.
Algorithm: 
		1.) While loop to check if input is within bounds.
********************************************************************* */
int player::handle_pre_turn_options() {

	cout << "1. Save the game" << endl;
	cout << "2. Make a move" << endl;
	if (this->name == "Human") {
		cout << "3. Ask for help" << endl;
	}
	cout << "4. Quit the game" << endl;
	cout << "What would you like to do? (option #): ";
	int pos;
	cin >> pos;
	int size = 4;

	while (!cin || (pos < 1 || pos > size)) {
		cout << "What would you like to do? (option #): ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> pos;
	}

	return pos;

}

/* *********************************************************************
Function Name: get_name
Purpose: Get player name
Parameters: None
Return Value: player name
Local Variables: none
Algorithm: none
Assistance Received: none
********************************************************************* */
string player::get_name() {
	return this->name;
}

/* *********************************************************************
Function Name: set_name
Purpose: To set a players name at any point outside the player class.
Parameters: sting, to set our name to another string name
Return Value: none
Local Variables: none
Algorithm: none
Assistance Received: none
********************************************************************* */
void player::set_name(string name) {
	this->name = name;
}

/* *********************************************************************
Function Name: get_hand
Purpose: Return the address of where the hand is, which can then remove or add to outside the class.
Parameters: none
Return Value: *vector<card>
Local Variables:
            temp[], an integer array used to sort the grades
Algorithm:
			1) Add all the grades
			2) Divide the sum by the number of students in class to calculate the average
Assistance Received: none
********************************************************************* */
vector<card>* player::get_hand() {
	return this->hand;
}


/* *********************************************************************
Function Name: get_capture_pile
Purpose: Return the address of where the capture pile is, which can then remove or add to outside the class.
Return Value: *vector<card>
Local Variables:
            temp[], an integer array used to sort the grades
Algorithm:
			1) Add all the grades
			2) Divide the sum by the number of students in class to calculate the average
Assistance Received: none
********************************************************************* */
vector<card>* player::get_capture_pile() {
	return this->capture_pile;
}


/* *********************************************************************
Function Name: get_help_move
Purpose: Heart and sole of a Strategy. The Computer uses this to play to win
Human can access it on their turn for what a computer would do if it had human hand.
Return Value: *vector<card>
Local Variables:
            card *card_to_place, pointer to which card in our hand we should select
			vecot<card> hand_matches, list of possible matches with our card
			vector<card> plr_hand, to defrence *this->get_hand()
			
Algorithm:
			1) For each layout_card in our layout and 
			2) For each hand_card in our hand
			3) if(hand_card.get_value() == layout_card.get_value() and layout_card.get_stack_children().size == 2 then we erturn our hand_card.get_slot_position() and suggest that or play if its computer calling get_help
			4) else 
			5) If our hand_card.value == layout_card.value then 
			6) We loop through our hand_matches to check if we have any other possible matches
			7) hand_matches is checked if we have a move for our current hand card, if we do we add the layout card to a possible pair
			8) after loops end we sort the hand_matches by descending order giving us index 0 as the best move with the most pairs made.
			9) output "Pairs can be created", "Triple stack can be captured", or no available moves
			10) hand_matches[0].get_stack_children() would return size and in which case if its != 0 we return its slot_position
			11) otherwise if we reach the end we have nothing availble and we output "nothing avaialble" and return 0
			
Assistance Received: none
********************************************************************* */
int player::get_help_move(vector<card> layout) {
	card *card_to_place;

	vector<card> hand_matches;


	//Loop through cards to find best possible matches

	vector<card> plr_hand = *this->get_hand();
	//
	for (card layout_card : layout) {
		for (int j = 0; j < plr_hand.size(); j++) {
			//check if we have a triple stack in the layout with our card matching if it does just capture that.
			card hand_card = plr_hand[j];
			hand_card.set_slot_position(j);
			//check if our hand card == layout card and we have a stack of 2 on this layout card
			//if so we return j as the best move immedietaly 
			if (hand_card.get_value() == layout_card.get_value() && layout_card.get_stack_children()->size() == 2) {

				cout << "The best move is to place your " << hand_card.get_name() << " with " << layout_card.get_name() << " for a 4 card capture." << endl;
				return j;
			}
			else {
				//pair found
				if (hand_card.get_value() == layout_card.get_value()) {
					bool found = false;
					//if we have a hand_match that contains hand_card then 
					//we add layout match to the children as another possible pair made 
					for(int i = 0; i < hand_matches.size(); i++) {
						if (hand_matches[i].get_value() == layout_card.get_value() && hand_card.get_value() == hand_matches[i].get_value()) {
							hand_matches[i].get_stack_children()->push_back(layout_card);
							found = true;
							break;
						}
					}
					//if we havent found it and we have a pair its our first pair.
					if (found == false) {
						hand_matches.push_back(hand_card);
						int slot = hand_matches.size() - 1;
						hand_matches[slot].get_stack_children()->push_back(layout_card);
					}
				}
			}
		}
	}
	//sort by higest pairs made
	sort(hand_matches.begin(), hand_matches.end(), [](card a, card b) {
		return b.get_stack_children()->size() < a.get_stack_children()->size();
		});

	//return why for what size the move collected.
	if (hand_matches.size() != 0) {
		vector<card> child = *hand_matches[0].get_stack_children();
		if (this->get_name() == "Computer") {
			cout << "Computer Played: " << hand_matches[0].get_name() << " on " << child[0].get_name() << " because";
			
			switch (hand_matches[0].get_stack_children()->size()) {
			case 0:
			case 1:
			case 2:
				cout << " it creates a pair" << endl;
				break;
			case 3:
				cout << " it can capture 4 cards" << endl;
				break;
			default: 
				cout << " it can capture 4 cards" << endl;
			}
		} else {
			cout << "You can play " << hand_matches[0].get_name() << " on " << child[0].get_name() << " because";
			switch (hand_matches[0].get_stack_children()->size()) {
			case 0:
			case 1:
			case 2:
				cout << " it creates a pair" << endl;
				break;
			case 3:
				cout << " it can capture 4 cards" << endl;
				break;
			default:
				cout << " it can capture 4 cards" << endl;
			}
		}
		return hand_matches[0].get_slot_position();;
	}
	if (this->get_name() == "Computer") {
		cout << "Computer played " << plr_hand[0].get_name() << " for no reason.";
	}
	else {
		cout << "Play any card since no good moves are avaialble."	 << endl;
	}
	return 0;
}


/* *********************************************************************
Function Name: get_capture_pile
Purpose: Return the address of where the capture pile is, which can then remove or add to outside the class.
Return Value: *vector<card>
Local Variables:
			temp[], an integer array used to sort the grades
Algorithm:
			1) Add all the grades
			2) Divide the sum by the number of students in class to calculate the average
Assistance Received: none
********************************************************************* */
void player::print_hand(bool in_option_menu) {
	cout << this->name << " hand: ";
	int i = 0;
	for (card cardtype : *this->hand) {
		if (this->name == "Human" && in_option_menu) {
			cout << "" << cardtype.get_name() << "(" << i << "), ";
		}
		else {
			cout << "" << cardtype.get_name() << ", ";
		}
		i++;
	}
	cout << endl;
}
void player::print_capture_pile() {
	cout << "Capture Pile: ";
	int i = 0;
	for (card cardtype : *this->capture_pile) {
		cout << "" << cardtype.get_name() << ", ";
	}
	cout << endl;
}

int player::get_score() {

	return this->score;
}

void player::generate_score() {
	map<CARD_VALUE, int> score_pairs = {};
	int temp_score = 0;

	for (card c : *this->get_capture_pile()) {
		CARD_VALUE val = static_cast<CARD_VALUE>(c.get_value());
		if (score_pairs.count(val)) {
			score_pairs[val]++;
			if (score_pairs[val] == 4) {
				temp_score += 1;
				score_pairs.erase(val);
			}
		}
		else {
			//if we dont have a possible match add it so we can make possible matches
			score_pairs.insert({ val,1 });
		}
	}

	this->score = this->score + temp_score;

}

player::~player() {
	delete this->hand;
}