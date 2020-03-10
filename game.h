/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/

#pragma once
#ifndef GAME_H
#define GAME_H

#include "gameround.h";
#include "human.h";
#include "computer.h"
#include <string>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;

class game
{
public:
	game();
	game(bool in_debug);
	~game();
	void start_game();

private:
	//Debug mode on
	bool in_debug;

	//Keep track of round state
	gameround current_round;

	//Both types of players
	human *human_player;
	computer *computer_player;



};


#endif