/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/


#include <iostream>
#include "game.h"
#include <filesystem>

int main()
{
	int option = -1;

	cout << "1. No" << endl;
	cout << "2. Yes" << endl;

	cout << "Welcome to Go Stop. Would you like to run this in debug mode? " << endl;

	cin >> option;
	int size = 2;
	while (!cin || (option < 1 || option > size)) {
		cout << "Welcome to Go Stop. Would you like to run this in debug mode? " << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> option;
	}


	game game((option == 1 ? false : true));

	game.start_game();

	system("pause");
}
