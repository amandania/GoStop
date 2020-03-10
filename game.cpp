/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/

#include "game.h"
/**
 * Default game Constructor
 * Default values 
 * debug = false, new human, new computer
 */
game::game() {
	this->in_debug = false;
	this->human_player = new human();
	this->computer_player = new computer();
}
/**
 * Constructor to set game debug mode
 */
game::game(bool in_debug) {
	this->in_debug = in_debug;
	this->human_player = new human();
	this->computer_player = new computer();
}

/**
 * Inital game start up 
 * Handles options before a round start.
 * 1. New game -> calls current_round->start(false)
 * 2. Load -> Choose a load() file to start the game
 * 3. Quit 
 */
void game::start_game() {
	this->current_round = gameround(this->in_debug, this->human_player, this->computer_player);

	int option = -1;

	cout << "What would you like to do?" << endl;

	cout << "1. Start New Game" << endl;
	cout << "2. Load" << endl;
	cout << "3. Quit" << endl;

	cin >> option;
	int size = 3;
	while (!cin || (option < 1 || option >= size)) {
		cout << "What would you like to do?";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> option;
	}

	switch (option) {
		//start the created round as new game.
		case 1:
			this->current_round.start_round(false);
			break;
		//load a game.
		case 2:
			current_round.load_game();
			break;
		//quit
		default:
			break;
	}

	this_thread::sleep_for(chrono::seconds(2));
	//Waits till final round is completed. 
	
}


/**
 * Destructor to delete pointers
 * Human pointer, computer pointer
 */
game::~game() {
	delete this->computer_player;
	delete this->human_player;
	this->in_debug = false;
}