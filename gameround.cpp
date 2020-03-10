/************************************************************
* Name:  Akshay Mandania                                    *
* Project : StopGo C++ Project							    *
* Class : CMPS 366 01 Organization of Programming Languages *
* Date : Feburary 11th, 2020								*
************************************************************/

#include <iostream>

#include "gameround.h"

gameround::gameround() {

}


gameround::gameround(bool in_debug, human *human_player, computer *computer_player) {
	this->in_debug = in_debug;
	this->human_player = human_player;
	this->computer_player = computer_player;
	this->round_count = 1;
}

void gameround::start_round(bool is_loaded) {
	if (is_loaded == false) {
		this->round_deck = new deck(2);
	}
	this->computer_player->set_game_layout(&this->layout);
	if (this->in_debug) {
		cout << "Deck Before shuffle\n";
		this->round_deck->print_deck();
		print_line_break(0);
	}
	if (is_loaded == false) {
		this->round_deck->shuffle_cards();
	}
	if (this->in_debug) {
		cout << "Deck After shuffle\n";
		this->round_deck->print_deck();
		print_line_break(0);

	}

	if (is_loaded == false) {
		//handles (5 to human and computer along with 4 to layout)x2
		deal_startup();
	}


	if (this->in_debug) {
		//this_thread::sleep_for(chrono::seconds(5));
	}
	cout << flush;
	system("CLS");

	if (is_loaded == false) {
		this->current_player = decide_first_player();
		if (this->current_player == nullptr) {
			cout << "Re-Dealing" << endl;
			this_thread::sleep_for(chrono::seconds(2));
			start_round(false);
		}
	}

	cout << "Round started.\n";
	cout << "Going first : " << current_player->get_name() << "\n";
	this->is_playing = true;
	while (this->is_playing) {
		print_line_break(1);
		computer_player->print_hand(true);
		computer_player->print_capture_pile();
		cout << "Score: " << computer_player->get_score() << endl;
		cout << endl;
		print_layout();
		cout << endl;
		human_player->print_hand(true);
		human_player->print_capture_pile();
		cout << "Score: " << human_player->get_score() << endl;
		print_line_break(2);
		int choice = current_player->handle_pre_turn_options();
		card chosen_card;
		switch (choice) {
			// save the game
		case 1:
			save_game();
			cout << "1. Yes" << endl;
			cout << "2. No" << endl;

			cout << "Would you like to continue playing?";
			choice = -1;
			cin >> choice;
			while (!cin || (choice < 1 || choice >= 3)) {
				cout << "Would you like to continue playing?";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cin >> choice;
			}
			if (choice == 2) {
				is_playing = false;
				cout << "Thank you for playing. Goodbye" << endl;
				return;
			}
			break;
			//help option for player only really.
		case 3:
			if (current_player->get_name() == "Human") {
				//this is the only time we can ask for help;
				current_player->get_help_move(this->layout);
			}
			break;
			//start turn
		case 2:
			chosen_card = current_player->handle_card_choice();
			print_line_break(0);
			add_card_to_layout(current_player, chosen_card);

			//next player
			if (current_player == this->human_player) {
				current_player = computer_player;
			}
			else {
				current_player = human_player;
			}
			//check for round ends
			if (current_player->get_hand()->size() == 0) {
				cout << current_player->get_name() << " has cleared his hand and ended the round.";
				cout << endl;
				end_round();
			}

			break;
		default:
			// quit the game
			cout << "Thank you for playing. Goodbye" << endl;
			is_playing = false;
			return;
			break;
		}
		
	}

}

void gameround::deal_from_deck(vector<card> *to_pile, const int amount, PileType pile_type) {
	if (pile_type == Layout) {
		add_cards_to_layout(this->round_deck->draw(amount));
	}
	else {
		add_cards_to_hand(this->round_deck->draw(amount), to_pile);
	}
}

void gameround::deal_startup() {
	for (int i = 0; i < 2; i++) {
		if (this->in_debug) {
			cout << "[" << this->human_player->get_name() << "] Hand size before dealing to: " << this->human_player->get_hand()->size() << " And deck size: " << this->round_deck->get_cards().size() << endl;
		}
		deal_from_deck(this->human_player->get_hand(), 5, Hand);
		if (this->in_debug) {
			cout << "Hand size after dealing: " << this->human_player->get_hand()->size() << " And deck size: " << this->round_deck->get_cards().size() << endl;
			print_line_break(0);
		}
		if (this->in_debug) {
			cout << "[" << this->computer_player->get_name() << "] Hand size before dealing to: " << this->computer_player->get_hand()->size() << " And deck size: " << this->round_deck->get_cards().size() << endl;
		}
		deal_from_deck(this->computer_player->get_hand(), 5, Hand);
		if (this->in_debug) {
			cout << "Hand size after dealing: " << computer_player->get_hand()->size() << " And deck size: " << this->round_deck->get_cards().size() << endl;

			print_line_break(0);
		}
		if (this->in_debug) {
			cout << "[GAME] Layout size before dealing to: " << this->layout.size() << " And deck size: " << this->round_deck->get_cards().size() << endl;
		}

		deal_from_deck(&this->layout, 4, Layout);
		if (this->in_debug) {
			cout << "[GAME] Layout size after dealing: " << this->layout.size() << " And deck size: " << this->round_deck->get_cards().size() << endl;

			print_line_break(0);
		}
	}
	this->computer_player->print_hand(false);
	this->human_player->print_hand(false);
	print_line_break(0);
}

void gameround::add_cards_to_hand(vector<card> cards, vector<card> *hand) {
	hand->insert(hand->begin(), cards.begin(), cards.end());
	sort_hand(hand, Hand);
}

void gameround::sort_hand(vector<card> *hand, PileType pile_type) {
	sort(hand->begin(), hand->end(), [](card a, card b) {
		return abs(b.get_value()) < abs(a.get_value());
		});
	if (pile_type == Layout) {
		for (int i = 0; i < this->layout.size(); i++) {
			this->layout[i].set_slot_position(i);
		}
	}
	return;
}

void gameround::add_card_to_layout(player *plr, card card_to_add) {
	vector<card> stack_found;
	int slotfound = -1;

	for (int i = 0; i < this->layout.size(); i++) {
		if (this->layout[i].get_value() == card_to_add.get_value()) {
			if (this->in_debug) {
				cout << "Found stack on " << card_to_add.get_name() << " With " << this->layout[i].get_name() << ".\n break out of loop, stack count is" << endl;
			}

			this->layout[i].set_slot_position(i);
			stack_found.push_back(this->layout[i]);
			slotfound = i;

			if (this->layout[i].get_stack_children()->size() == 2) {
				//triple stack found.
				break;
			}
		}
		else if (this->in_debug) {
			cout << "cant find stack\n";
		}
	}

	vector<card> capture_stack;
	card chosen_card;
	//in case 2
	card drawn_card;
	vector<card> my_draw_pairs;
	int slot_for_draw_pair = -1;


	if (slotfound != -1) {
		card original_layout = this->layout[slotfound];
		card layout_card = this->layout[slotfound];
		//Indiviudal unique layout cards
		switch (stack_found.size()) {
			//Matches with 1 cards in the layout
		case 1:
			//if our paired card is a triple stack capture all.
			if (layout_card.get_stack_children()->size() == 2) {
				//Create a vector of capture cards of all the cards
				//Add them to capture pile of player
				cout << plr->get_name() << " captured a triple stack with: " << card_to_add.get_name() << " and  " << layout_card.get_name() << ", ";
				vector<card> layout = *layout_card.get_stack_children();
				for (card c : layout) {

					if (this->in_debug) {
						cout << " add " << c.get_name() << " to capture pile" << endl;
					}
					cout << c.get_name() << ", ";
					capture_stack.push_back(c);
				}
				cout << endl;
				capture_stack.push_back(layout_card);
				capture_stack.push_back(card_to_add);
				if (this->in_debug) {
					cout << " erase element at : " << slotfound << endl;
				}
				add_to_capture_pile(capture_stack, plr);
				this->layout.erase(this->layout.begin() + slotfound);

			}
			else {
				//single pair made with chosen card.

				this->layout[slotfound].get_stack_children()->push_back(card_to_add);
			}

			capture_stack.clear();
			//draw a card and handle draw turn
			drawn_card = this->round_deck->draw();
			print_line_break(0);
			cout << plr->get_name() << " drew a " << drawn_card.get_name() << endl;
			//check if we have any cases matched to layout for card
			for (int i = 0; i < this->layout.size(); i++) {
				if (this->layout[i].get_value() == drawn_card.get_value()) {
					if (this->in_debug) {
						cout << "Found stack on " << drawn_card.get_name() << " With " << this->layout[i].get_name() << ".\n" << endl;
					}

					this->layout[i].set_slot_position(i);
					my_draw_pairs.push_back(this->layout[i]);
					slot_for_draw_pair = i;

					if (this->layout[i].get_stack_children()->size() == 2) {
						//triple stack found.
						break;
					}
				}
				else if (this->in_debug) {
					cout << "cant find stack\n";
				}
			}
			if (slot_for_draw_pair != -1) {
				layout_card = this->layout[slot_for_draw_pair];
				if (layout_card.get_stack_children()->size() == 2) {

					cout << plr->get_name() << " Captured a triple stack of " << drawn_card.get_name() << ", " << layout_card.get_name() << ", ";

					vector<card> layout = *layout_card.get_stack_children();
					for (card c : layout) {

						if (this->in_debug) {
							cout << " add " << c.get_name() << " to capture pile" << endl;
						}
						cout << c.get_name() << ", ";
						capture_stack.push_back(c);
					}
					cout << endl;
					capture_stack.push_back(layout_card);
					capture_stack.push_back(drawn_card);

					capture_stack.push_back(card_to_add);
					capture_stack.push_back(original_layout);


					cout << "Along with original "<<(original_layout.get_stack_children()->size() == 2 ? "triple stack" : "pair") <<" of " << card_to_add.get_name() << ""<< (original_layout.get_stack_children()->size() == 2 ? "," : "and") <<" " << original_layout.get_name();
					if (original_layout.get_stack_children()->size() == 2) {
						cout << ", ";
						for (card c : *original_layout.get_stack_children()) {
							cout << c.get_name() << ", ";
						}
					}
					cout << endl;
					add_to_capture_pile(capture_stack, plr);

					this->layout.erase(this->layout.begin() + slot_for_draw_pair);
				}
				else {
					//Handle the draw pair conditions
					switch (my_draw_pairs.size()) {
						//capture both the card drawn and the paired card
					case 2:
					case 1:
						//if our draw card is not equal to layout card and we have a 1 card that matches in the layout
						//capture both
						if (drawn_card.get_value() != original_layout.get_value()) {
							capture_stack.clear();
							capture_stack.push_back(original_layout);
							capture_stack.push_back(drawn_card);
							capture_stack.push_back(layout_card);
							capture_stack.push_back(card_to_add);

							add_to_capture_pile(capture_stack, plr);
							cout << plr->get_name() << " captured original pair of " << original_layout.get_name() << " and " << card_to_add.get_name() << " along with drawn pair of " << drawn_card.get_name() << " and " << layout_card.get_name() << endl;

							int last = 0;
							for (int i = 0; i < this->layout.size(); ++i, ++last)
							{
								while (i == original_layout.get_slot_position() || i == layout_card.get_slot_position())
									++i;
								if (i >= this->layout.size()) break;

								this->layout[last] = this->layout[i];
							}

							this->layout.resize(last);
							/*
							this->layout.erase(this->layout.begin() + original_layout.get_slot_position());


							this->layout.erase(this->layout.begin() + my_draw_pairs[0].get_slot_position());
							*/
							/*
							if (layout_card.get_slot_position() == this->layout.size()) {
								this->layout.pop_back();
							}
							else {
								this->layout.erase(this->layout.begin() + layout_card.get_slot_position());
							}*/
						}
						else {
							//if its equal to original card it means we have a pair created with 1 stack, thus leave all three as a triple stack.
							this->layout[original_layout.get_slot_position()].get_stack_children()->push_back(drawn_card);
							cout << plr->get_name() << " created a triple stack with " << original_layout.get_name() << ", " << card_to_add.get_name() << " and " << drawn_card.get_name() << endl;
						}
						break;
						//triple cpatures and original pair captures
					default:
						capture_stack.clear();
						capture_stack.push_back(drawn_card);
						cout << plr->get_name() << " Captured Three Cards " << drawn_card.get_name() << ", ";
						capture_stack.push_back(card_to_add);
						capture_stack.push_back(original_layout);
						int last = 0;
						for (int i = 0; i < (my_draw_pairs.size() > 4 ? 4 : my_draw_pairs.size()); i++) {
							cout << my_draw_pairs[i].get_name() << ", ";
							capture_stack.push_back(my_draw_pairs[i]);
							

							auto pos = this->layout.begin();
							for (auto j = this->layout.begin(); j != this->layout.end(); ++j)
							{
								if (j->get_slot_position() != my_draw_pairs[i].get_slot_position()) // whatever condition...
								{
									*pos++ = *j; // will move, if move assignment is available...
								}
							}

							this->layout.erase(pos, this->layout.end());
						}

						cout << "Along with original pair of " << card_to_add.get_name() << " and " << original_layout.get_name() << endl;
						add_to_capture_pile(capture_stack, plr);
						//this->layout.resize(last);
						this->layout.erase(this->layout.begin() + original_layout.get_slot_position());
						cout << endl;
						break;
					}
				}
			}
			else {
				//we will have a pair made here since we are still in H1 case
				//draw card had no pairs.
				//just capture original H1 Case
				//make sure to check if this wasnt a triple stack capture already.
				if (layout_card.get_stack_children()->size() != 2) {
					capture_stack.push_back(layout_card);
					capture_stack.push_back(card_to_add);
					add_to_capture_pile(capture_stack, plr);
					cout << plr->get_name() << " captured original pair of " << card_to_add.get_name() << " and " << layout_card.get_name() << endl;
					if (layout_card.get_slot_position() == this->layout.size()) {
						this->layout.pop_back();
					}
					else {
						this->layout.erase(this->layout.begin() + layout_card.get_slot_position());
					}
				}

				sort_hand(&this->layout, Layout);


				cout << drawn_card.get_name() << " was added to the layout" << endl;;
				this->layout.push_back(drawn_card);
			}
			break;
			//Matches with 2 cards in the layout
		case 2:
			//Player picks from one of two of the cards to create a stack pair with.
			chosen_card = plr->choose_which_stack(stack_found);
			slotfound = chosen_card.get_slot_position();
			//If we have chose a card with x2 children making it a triple stack
			//parent + 2 child
			//capture all 4.

			if (chosen_card.get_stack_children()->size() == 2) {
				cout << plr->get_name() << " captured a triple stack with: " << card_to_add.get_name() << " and  " << layout_card.get_name() << ", ";
				vector<card> layout = *chosen_card.get_stack_children();
				for (card c : layout) {

					if (this->in_debug) {
						cout << " add " << c.get_name() << " to capture pile" << endl;
					}
					cout << c.get_name() << ", ";
					capture_stack.push_back(c);
				}
				cout << endl;
				capture_stack.push_back(chosen_card);
				capture_stack.push_back(card_to_add);

				add_to_capture_pile(capture_stack, plr);
				if (this->in_debug) {
					cout << " erase element at : " << slotfound << endl;
				}
				this->layout.erase(this->layout.begin() + chosen_card.get_slot_position());
			}
			else {
				//single pair made with chosen card.
				//this->layout[slotfound].get_stack_children()->push_back(card_to_add);
			}
			capture_stack.clear();
			drawn_card = this->round_deck->draw();
			print_line_break(0);
			cout << plr->get_name() << " drew a " << drawn_card.get_name() << endl;
			//check if we have any cases matched to layout for card
			for (int i = 0; i < this->layout.size(); i++) {
				if (this->layout[i].get_value() == drawn_card.get_value()) {
					if (this->in_debug) {
						cout << "Found stack on " << drawn_card.get_name() << " With " << this->layout[i].get_name() << ".\n" << endl;
					}

					this->layout[i].set_slot_position(i);
					my_draw_pairs.push_back(this->layout[i]);
					slot_for_draw_pair = i;

					if (this->layout[i].get_stack_children()->size() == 2) {
						//triple stack found.
						break;
					}
				}
				else if (this->in_debug) {
					cout << "cant find stack\n";
				}
			}
			if (slot_for_draw_pair != -1) {
				layout_card = this->layout[slot_for_draw_pair];
				//triple stack found
				if (layout_card.get_stack_children()->size() == 2) {
					//capture all cards draw/children and layout
					cout << plr->get_name() << " Captured Triple stack with " << drawn_card.get_name() << ", " << layout_card.get_name() << ", ";
					for (card c : *layout_card.get_stack_children()) {
						cout << c.get_name() << ", ";
						capture_stack.push_back(c);
					}
					cout << endl << "Along with original pair of " << card_to_add.get_name() << " and " << chosen_card.get_name() << endl;
					capture_stack.push_back(drawn_card);
					capture_stack.push_back(layout_card);
					capture_stack.push_back(card_to_add);
					capture_stack.push_back(chosen_card);
					
					int last = 0;
					for (int i = 0; i < this->layout.size(); ++i, ++last)
					{
						while (i == original_layout.get_slot_position() || i == layout_card.get_slot_position())
							++i;
						if (i >= this->layout.size()) break;

						this->layout[last] = this->layout[i];
					}

					this->layout.resize(last);

					capture_stack.clear();
				}
				else {
					switch (my_draw_pairs.size()) {
						//capture both the card drawn and the paired card
					case 1:
					case 2:
						//if our draw card is not equal to layout card and we have a 1 card that matches in the layout
						//capture both paris
						/*sort(my_draw_pairs.begin(), my_draw_pairs.end(), [](card a, card b) {
							return b.get_stack_children()->size() < a.get_stack_children()->size();
							});
						layout_card = my_draw_pairs[0];*/
						if (drawn_card.get_value() != original_layout.get_value()) {
							capture_stack.clear();
							capture_stack.push_back(original_layout);
							capture_stack.push_back(drawn_card);
							capture_stack.push_back(layout_card);
							capture_stack.push_back(card_to_add);
							cout << plr->get_name() << " captured original pair of " << chosen_card.get_name() << " and " << card_to_add.get_name() << " along with drawn pair of " << drawn_card.get_name() << " and " << layout_card.get_name() << endl;

							add_to_capture_pile(capture_stack, plr);
							int last = 0;
							for (int i = 0; i < this->layout.size(); ++i, ++last)
							{
								while (i == chosen_card.get_slot_position() || i == layout_card.get_slot_position())
									++i;
								if (i >= this->layout.size()) break;

								this->layout[last] = this->layout[i];
							}

							this->layout.resize(last);

						}
						else {
							//if its equal to original card it means we have a pair created with 1 stack, thus leave all three as a triple stack.
							this->layout[chosen_card.get_slot_position()].get_stack_children()->push_back(drawn_card);
							this->layout[chosen_card.get_slot_position()].get_stack_children()->push_back(card_to_add);
							cout << plr->get_name() << " created a triple stack with " << chosen_card.get_name() << ", " << card_to_add.get_name() << " and " << drawn_card.get_name() << endl;
						}
						break;
						//triple cpatures and original pair captures
					default:
						capture_stack.push_back(drawn_card);
						cout << plr->get_name() << " Captured Triple stack with " << drawn_card.get_name() << ", ";
						for (int i = 0; i < 4; i++) {
							cout << my_draw_pairs[i].get_name() << ", ";
							capture_stack.push_back(my_draw_pairs[i]);
							this->layout.erase(this->layout.begin() + my_draw_pairs[i].get_slot_position());
						}
						capture_stack.push_back(card_to_add);
						capture_stack.push_back(original_layout);
						add_to_capture_pile(capture_stack, plr);
						this->layout.erase(this->layout.begin() + original_layout.get_slot_position());
						cout << endl;
						break;
					}
				}
			}
			else {
				//we will have a pair made here since we are still in H1 case
				//draw card had no pairs.
				//just capture original H1 Case
				if (layout_card.get_stack_children()->size() != 2) {
					capture_stack.push_back(chosen_card);
					capture_stack.push_back(card_to_add);
					add_to_capture_pile(capture_stack, plr);
					cout << plr->get_name() << " captured original pair of " << card_to_add.get_name() << " and " << chosen_card.get_name() << endl;
					this->layout.erase(this->layout.begin() + chosen_card.get_slot_position());
				}


				cout << drawn_card.get_name() << " was added to the layout" << endl;
				this->layout.push_back(drawn_card);
			}
			break;
			//Matches with 3 or more cards in the layout
		default:
			//If we have 3 matches then we collect all 3 + our card into capture pile.
			cout << plr->get_name() << " made a triple stack capture of: " << card_to_add.get_name() << ", ";
			for (card c : stack_found) {
				//remove from our layout
				cout << c.get_name() << ", ";
				capture_stack.push_back(c);

				//just using a loop because c.getslot_pos does not get updated 
				//while erase updates layout and slot positions
				for (int i = 0; i < this->layout.size(); i++) {
					if (this->layout[i].get_value() == c.get_value()) {
						this->layout.erase(this->layout.begin() + i);
					}
				}
				if (capture_stack.size() == 4) {
					//capture first 4 only.
					break;
				}
			}
			cout << endl;

			//add our card placed to the stack
			capture_stack.push_back(card_to_add);
			add_to_capture_pile(capture_stack, plr);
			capture_stack.clear();
			//draw phase same rules as h0 apply here

			drawn_card = this->round_deck->draw();
			print_line_break(0);
			cout << plr->get_name() << " drew a " << drawn_card.get_name() << endl;
			//check if we have any cases matched to layout for card
			for (int i = 0; i < this->layout.size(); i++) {
				if (my_draw_pairs.size() == 3) {
					break;
				}
				if (this->layout[i].get_value() == drawn_card.get_value()) {
					if (this->in_debug) {
						cout << "Found stack on " << drawn_card.get_name() << " With " << this->layout[i].get_name() << ".\n" << endl;
					}

					this->layout[i].set_slot_position(i);
					my_draw_pairs.push_back(this->layout[i]);
					slot_for_draw_pair = i;
					//break if triple stakc exists at this slot.
					if (this->layout[i].get_stack_children()->size() == 2) {
						break;
					}
				}
				else if (this->in_debug) {
					cout << "cant find stack\n";
				}
			}


			if (slot_for_draw_pair == -1) {
				//nothing was matched to draw pair.
				//add draw card to layout
				cout << "drawn card: " << drawn_card.get_name() << " was placed into the layout." << endl;
				this->layout.push_back(drawn_card);
				sort_hand(&this->layout, Layout);
			}
			else {
				// we have some kind of match.
				switch (my_draw_pairs.size()) {
					//our draw card matches 1 card
				case 1:
					if (this->layout[slot_for_draw_pair].get_stack_children()->size() == 2) {
						//capture all 4

						cout << plr->get_name() << " made a triple stack capture of " << this->layout[slot_for_draw_pair].get_name() << ", " << drawn_card.get_name();
						for (card c : *this->layout[slot_for_draw_pair].get_stack_children()) {
							cout << c.get_name() << ", ";
							capture_stack.push_back(c);
						}
						cout << endl;

						capture_stack.push_back(this->layout[slot_for_draw_pair]);
						capture_stack.push_back(drawn_card);

						add_to_capture_pile(capture_stack, plr);
						this->layout.erase(this->layout.begin() + slot_for_draw_pair);
					}
					else {
						cout << plr->get_name() << " captured " << this->layout[slot_for_draw_pair].get_name() << " and " << drawn_card.get_name() << endl;
						capture_stack.push_back(this->layout[slot_for_draw_pair]);
						capture_stack.push_back(drawn_card);
						add_to_capture_pile(capture_stack, plr);
						this->layout.erase(this->layout.begin() + slot_for_draw_pair);
					}
					break;
					//pick from the two cards that match draw card and 
					//capture both stock card and matching choice
				case 2:
					chosen_card = plr->choose_which_stack(my_draw_pairs);


					if (chosen_card.get_stack_children()->size() == 2) {
						cout << plr->get_name() << " made a triple stack capture of " << chosen_card.get_name() << ", " << drawn_card.get_name();
						for (card c : *chosen_card.get_stack_children()) {
							cout << c.get_name() << ", ";
							capture_stack.push_back(c);
						}
						capture_stack.push_back(chosen_card);
						capture_stack.push_back(drawn_card);
						add_to_capture_pile(capture_stack, plr);
						this->layout.erase(this->layout.begin() + chosen_card.get_slot_position());
						cout << endl;
					}
					else {

						capture_stack.push_back(chosen_card);
						capture_stack.push_back(drawn_card);
						add_to_capture_pile(capture_stack, plr);
						cout << plr->get_name() << " captured " << chosen_card.get_name() << " and " << drawn_card.get_name() << endl;

						this->layout.erase(this->layout.begin() + chosen_card.get_slot_position());
					}

					break;
					// if our draw card has 3 cards in layout capture all 4 cards
				default:
					cout << plr->get_name() << "made a triple stack capture of " << drawn_card.get_name() << ", ";
					for (card c : my_draw_pairs) {
						if (capture_stack.size() == 4) {
							//capture first 4 only.
							break;
						}
						cout << c.get_name() << ", ";
						capture_stack.push_back(c);
						this->layout.erase(this->layout.begin() + c.get_slot_position());
					}
					cout << endl;
					capture_stack.push_back(drawn_card);
					add_to_capture_pile(capture_stack, plr);

					break;
				}
			}
			break;
		}
	}
	else {
		//cout << "No pairs or stacks where availbale. Just add to layout." << endl;
		//draw a card and have same case for h3 apply here aswell
		//draw a card and handle draw turn

		this->layout.push_back(card_to_add);
		cout << plr->get_name() << " added " << card_to_add.get_name() << " to the layout." << endl;
		sort_hand(&this->layout, Layout);

		drawn_card = this->round_deck->draw();
		print_line_break(0);
		cout << plr->get_name() << " drew a " << drawn_card.get_name() << endl;
		//check if we have any cases matched to layout for card
		for (int i = 0; i < this->layout.size(); i++) {
			if (my_draw_pairs.size() == 3) {
				break;
			}
			if (this->layout[i].get_value() == drawn_card.get_value()) {
				if (this->in_debug) {
					cout << "Found stack on " << drawn_card.get_name() << " With " << this->layout[i].get_name() << ".\n" << endl;
				}

				this->layout[i].set_slot_position(i);
				my_draw_pairs.push_back(this->layout[i]);
				slot_for_draw_pair = i;
				//check triple stack and break;

				if (this->layout[i].get_stack_children()->size() == 2) {
					//triple stack found.
					break;
				}
			}
			else if (this->in_debug) {
				cout << "cant find stack\n";
			}
		}


		if (slot_for_draw_pair == -1) {
			//nothing was matched to draw pair.
			//add draw card to layout
			cout << " drawn card: " << drawn_card.get_name() << " was placed into the layout." << endl;
			this->layout.push_back(drawn_card);
			sort_hand(&this->layout, Layout);
		}
		else {
			// we have some kind of match.
			switch (my_draw_pairs.size()) {
				//our draw card matches 1 card
			case 1:
				if (this->layout[slot_for_draw_pair].get_stack_children()->size() == 2) {
					//capture all 4

					cout << plr->get_name() << " captured " << this->layout[slot_for_draw_pair].get_name() << ", " << drawn_card.get_name();
					for (card c : *this->layout[slot_for_draw_pair].get_stack_children()) {
						cout << c.get_name() << ", ";
						capture_stack.push_back(c);
					}
					cout << endl;

					capture_stack.push_back(this->layout[slot_for_draw_pair]);
					capture_stack.push_back(drawn_card);

					add_to_capture_pile(capture_stack, plr);
					this->layout.erase(this->layout.begin() + slot_for_draw_pair);
				}
				else {
					cout << plr->get_name() << " captured " << this->layout[slot_for_draw_pair].get_name() << " and " << drawn_card.get_name() << endl;
					capture_stack.push_back(this->layout[slot_for_draw_pair]);
					capture_stack.push_back(drawn_card);
					add_to_capture_pile(capture_stack, plr);
					this->layout.erase(this->layout.begin() + slot_for_draw_pair);
				}
				break;
				//pick from the two cards that match draw card and 
				//capture both stock card and matching choice
			case 2:
				chosen_card = plr->choose_which_stack(my_draw_pairs);
				capture_stack.push_back(chosen_card);
				capture_stack.push_back(drawn_card);
				add_to_capture_pile(capture_stack, plr);
				cout << plr->get_name() << " captured " << chosen_card.get_name() << " and " << drawn_card.get_name() << endl;
				this->layout.erase(this->layout.begin() + chosen_card.get_slot_position());

				break;
				// if our draw card has 3 cards in layout capture all 4 cards
			case 3:
				cout << plr->get_name() << "made a triple stack capture of " << drawn_card.get_name() << ", ";
				for (card c : my_draw_pairs) {
					if (capture_stack.size() == 4) {
						//capture first 4 only.
						break;
					}
					cout << c.get_name() << ", ";
					capture_stack.push_back(c);
					//this->layout.erase(this->layout.begin() + c.get_slot_position());

					auto pos = this->layout.begin();
					for (auto i = this->layout.begin(); i != this->layout.end(); ++i)
					{
						if (i->get_slot_position() != c.get_slot_position()) // whatever condition...
						{
							*pos++ = *i; // will move, if move assignment is available...
						}
					}

					this->layout.erase(pos, this->layout.end());
				}

				cout << endl;
				capture_stack.push_back(drawn_card);
				add_to_capture_pile(capture_stack, plr);

				break;
			}
		}
	}


	sort_hand(&this->layout, Layout);
}

void gameround::add_cards_to_layout(vector<card> cards) {
	this->layout.insert(this->layout.begin(), cards.begin(), cards.end());
	sort_hand(&this->layout, Layout);
}

player *gameround::decide_first_player() {
	vector<card> human_hand = *this->human_player->get_hand();
	vector<card> computer_hand = *this->computer_player->get_hand();

	for (int i = 0; i < human_hand.size(); i++) {
		if (human_hand[i].get_value() == computer_hand[i].get_value()) {
			continue;
		}
		if (human_hand[i].get_value() > computer_hand[i].get_value()) {
			// human goes first.
			return this->human_player;
		}
		else {
			// computer goes first.
			return this->computer_player;
		}
	}
	return nullptr;
}

void gameround::print_line_break(int option) {
	cout << "---------------------------------------------------" << (option == 1 ? ("Round: " + to_string(this->round_count)) : option == 2 ? ("Stock: " + this->round_deck->get_cards()[0].get_name()) : "") << "----------------------------------------------------------" << endl;
}

void gameround::print_layout() {
	cout << "LAYOUT:" << endl;
	vector<card> card_parents = vector<card>(this->layout.size());


	for (card cardtype : this->layout) {
		cout << "" << cardtype.get_name() << ", ";
		if (cardtype.get_stack_children()->size() > 0) {
			int parent_index = cardtype.get_slot_position();

			card_parents[parent_index] = cardtype;
			for (card child : *cardtype.get_stack_children()) {
				child.set_slot_position(parent_index);
			}
		}
	}
	int max_lines = get_highest_child_card().get_stack_children()->size();
	if (max_lines > 0) {
		cout << endl;
		for (int j = 0; j < max_lines; j++) {
			for (int i = 0; i < this->layout.size(); i++) {
				vector<card> &def = *card_parents[i].get_stack_children();

				if (def.size() > 0) {

				}
				cout << (card_parents[i].get_stack_children()->size() > 0 && card_parents[i].get_stack_children()->size() > j ? (def[j].get_name()) : "  ") << "  ";
			}
			cout << endl;
		}
	}

	cout << endl;
}

card gameround::get_highest_child_card() {
	vector<card> clone = this->layout;
	sort(clone.begin(), clone.end(), [](card a, card b) {
		return b.get_stack_children()->size() < a.get_stack_children()->size();
		});


	return clone[0];
}

void gameround::add_to_capture_pile(vector<card> capture_cards, player *player) {
	if (this->in_debug == true) {
		cout << player->get_name() << " captured x" << capture_cards.size() << " cards." << endl;
	}
	for (card c : capture_cards) {
		if (this->in_debug) {
			cout << player->get_name() << " added " << c.get_name() << " to capture pile" << endl;
		}
		player->get_capture_pile()->push_back(c);
	}

	sort_hand(player->get_capture_pile(), Hand);
}

void gameround::save_game() {
	string default_save = "save_session";


	fs::path path(fs::current_path().string() + "/saves/");
	int i = 1;
	vector<fs::path> entry_list;
	for (const auto & entry : fs::directory_iterator(path)) {
		cout << i << ". " << entry.path().filename().string() << endl;
		entry_list.push_back(entry);
		i++;
	}
	default_save += (entry_list.size() < 10 ? "_0" : "_") + to_string(entry_list.size() + 1);
	ofstream myfile;

	myfile.open("saves/" + default_save + ".txt", ios::app);
	myfile << "Round: " << this->round_count << endl << endl;
	myfile << "Computer:" << endl;
	myfile << "   Score: " << this->computer_player->get_score() << endl;
	myfile << "   Hand: ";
	for (card c : *this->computer_player->get_hand()) {

		myfile << (c.get_name().substr(0, 2) == "10" ? ("X" + c.get_name().substr(2, 3)) : c.get_name()) << " ";
	}
	myfile << endl;
	myfile << "   Capture Pile: ";
	for (card c : *this->computer_player->get_capture_pile()) {

		myfile << (c.get_name().substr(0, 2) == "10" ? ("X" + c.get_name().substr(2, 3)) : c.get_name()) << " ";
	}
	myfile << endl << endl;

	myfile << "Human:" << endl;
	myfile << "   Score: " << this->human_player->get_score() << endl;
	myfile << "   Hand: ";
	for (card c : *this->human_player->get_hand()) {

		myfile << (c.get_name().substr(0, 2) == "10" ? ("X" + c.get_name().substr(2, 3)) : c.get_name()) << " ";
	}
	myfile << endl;
	myfile << "   Capture Pile: ";
	for (card c : *this->human_player->get_capture_pile()) {

		myfile << (c.get_name().substr(0, 2) == "10" ? ("X" + c.get_name().substr(2, 3)) : c.get_name()) << " ";
	}
	myfile << endl << endl;

	myfile << "Layout: ";
	for (card c : this->layout) {
		myfile << (c.get_name().substr(0, 2) == "10" ? ("X" + c.get_name().substr(2, 3)) : c.get_name()) << " ";
	}

	myfile << endl << endl;

	myfile << "Stock Pile: ";
	for (card c : this->round_deck->get_cards()) {
		myfile << (c.get_name().substr(0, 2) == "10" ? ("X" + c.get_name().substr(2, 3)) : c.get_name()) << " ";
	}

	myfile << endl << endl;

	myfile << "Next Player: " << this->current_player->get_name() << endl;


	cout << "You game session has been saved" << (this->in_debug == true ? default_save : "") << endl;
	myfile.close();
}

vector<string> gameround::split_string(const string &s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
		// elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
	}
	return elems;
}

void gameround::load_game() {
	//
	int computer_score;
	int human_score;
	vector<card> computer_hand;
	vector<card> computer_capture;
	vector<card> human_hand;
	vector<card> human_capture;
	vector<card> layout_cards;

	vector<card> deck_cards;


	fs::path path(fs::current_path().string() + "/saves/");
	int i = 1;
	int pos = -1;
	vector<fs::path> entry_list;
	for (const auto & entry : fs::directory_iterator(path)) {
		cout << i << ". " << entry.path().filename().string() << endl;
		entry_list.push_back(entry);
		i++;
	}
	cout << "Which game file would you like to load? : ";
	cin >> pos;
	int size = i;

	while (!cin || (pos < 1 || pos >= size)) {
		cout << "Which game file would you like to load? : ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> pos;
	}

	cout << "Loading gamefile:" << entry_list[pos - 1].filename().string() << endl;

	//read file
	string line;
	ifstream MyReadFile("saves/" + entry_list[pos - 1].filename().string());

	while (getline(MyReadFile, line)) {
		cout << line << endl;
	}
	MyReadFile.close();

	MyReadFile = ifstream("saves/" + entry_list[pos - 1].filename().string());

	while (getline(MyReadFile, line)) {
		// Output the text from the file
		string temp = line;
		stringstream check1(temp);
		string p;
		vector <string> split;

		if (temp.substr(0, 4) == "Comp") {
			//read next 3 lines for score hand and capture
			string score;
			string hand;
			string capture;
			getline(MyReadFile, score);
			split = split_string(score, ':');
			int value = stoi(split[1]);
			computer_score = value;
			getline(MyReadFile, hand);
			split = split_string(hand, ':');
			split = split_string(split[1], ' ');
			split.erase(split.begin());
			computer_hand = parse_cards(split);
			//cout << endl;
			getline(MyReadFile, capture);
			split = split_string(capture, ':');
			if (split.size() > 1) {
				split = split_string(split[1], ' ');
				split.erase(split.begin());
				computer_capture = parse_cards(split);
			}
			else {
				computer_capture = vector<card>();
			}
			//cout << endl;

			split = split_string(capture, ':');

			//cout << endl;
		}
		else if (temp.substr(0, 4) == "Huma") {
			string score;
			string hand;
			string capture;
			getline(MyReadFile, score);
			split = split_string(score, ':');
			int value = stoi(split[1]);
			human_score = value;

			getline(MyReadFile, hand);

			split = split_string(hand, ':');

			split = split_string(split[1], ' ');
			split.erase(split.begin());
			human_hand = parse_cards(split);
			//cout << endl;

			getline(MyReadFile, capture);

			split = split_string(capture, ':');

			if (split.size() > 1) {
				split = split_string(split[1], ' ');
				split.erase(split.begin());
				human_capture = parse_cards(split);
				//cout << endl;

				split = split_string(capture, ':');
			}
			else {
				human_capture = vector<card>();
			}

			cout << endl;
		}
		else if (temp.substr(0, 4) == "Layo") {
			split = split_string(temp, ':');

			split = split_string(split[1], ' ');
			split.erase(split.begin());
			layout_cards = parse_cards(split);

		}
		else if (temp.substr(0, 4) == "Stoc") {
			split = split_string(temp, ':');
			if (split.size() > 1) {
				split = split_string(split[1], ' ');
				split.erase(split.begin());
				deck_cards = parse_cards(split);
			}

		}
		else if (temp.substr(0, 4) == "Next") {
			split = split_string(temp, ':');

			split = split_string(split[1], ' ');

			cout << "Next Player is: " << split[1] << endl;
			if (split[1] == "Human") {
				this->current_player = this->human_player;
			}
			else {
				this->current_player = this->computer_player;
			}
		}
	}

	cout << endl << "deck cards: " << endl;

	add_cards_to_hand(computer_hand, this->computer_player->get_hand());
	add_cards_to_hand(human_hand, this->human_player->get_hand());
	add_cards_to_layout(layout_cards);
	add_to_capture_pile(computer_capture, this->computer_player);
	add_to_capture_pile(human_capture, this->human_player);

	this->round_deck = new deck(2);
	this->round_deck->clear_deck();

	cout << "round deck size = " << this->round_deck->get_cards().size() << endl;


	i = 0;
	for (card cardtype : deck_cards) {
		cout << "" << cardtype.get_name() << ", ";
		i++;
		this->round_deck->add_card(cardtype);
	}


	cout << endl << "round deck size = " << this->round_deck->get_cards().size() << endl;

	start_round(true);
	cout << endl;
}

vector<card> gameround::parse_cards(vector<string> cards) {
	vector<card> to_return;



	for (int i = 0; i < cards.size(); i++) {
		//our split length should know be 2 X == 10(2 space character)
		string card_name = cards[i];
		string cardsuitesplit = cards[i];
		CARD_VALUE val = Two;
		CARD_SUITE suite = Heart;


		if (card_name.substr(0, 1) == "X") {
			val = Ten;
		}
		else if (card_name.substr(0, 1) == "J") {
			val = Jack;
		}
		else if (card_name.substr(0, 1) == "Q") {
			val = Queen;
		}
		else if (card_name.substr(0, 1) == "K") {
			val = King;
		}
		else if (card_name.substr(0, 1) == "A") {
			val = Ace;
		}
		else {
			val = static_cast<CARD_VALUE>(stoi(card_name.substr(0, 1)));
		}


		if (cardsuitesplit.substr(1, 2) == "H") {
			suite = Heart;
		}
		else if (cardsuitesplit.substr(1, 2) == "S") {
			suite = Spade;
		}
		else if (cardsuitesplit.substr(1, 2) == "C") {
			suite = Club;
		}
		else if (cardsuitesplit.substr(1, 2) == "D") {
			suite = Diamond;
		}

		to_return.push_back(card(val, suite));


	}


	return to_return;
}

void gameround::end_round() {
	this->computer_player->generate_score();
	this->human_player->generate_score();
	cout << "You " << ((this->human_player->get_score() > this->computer_player->get_score()) ? "win the" : (this->human_player->get_score() < this->computer_player->get_score()) ? " loose the" : "tie this") << " round with a score of " << this->human_player->get_score() << " .vs. " << this->computer_player->get_score() << endl;

	cout << "1. Play New Round " << endl;
	cout << "2. Quit and Save Game" << endl;
	cout << "What would you like to do now? ";;
	int pos = -1;
	cin >> pos;
	int size = 2;

	while (!cin || (pos < 1 || pos >= size)) {
		cout << "What would you like to do now? ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> pos;
	}

	cout << endl;
	switch (pos) {
		// start new round
	case 1:
		this->round_count++;
		cout << "Round " << this->round_count << " is about to begin..." << endl << flush;

		this_thread::sleep_for(chrono::seconds(2));
		system("CLS");
		this->human_player->get_hand()->clear();
		this->human_player->get_capture_pile()->clear();
		this->computer_player->get_hand()->clear();
		this->layout.clear();
		this->computer_player->get_capture_pile()->clear();
		delete this->round_deck;
		start_round(false);
		break;

	//Quit the game
	case 2:
		cout << endl;
		save_game();
		if (pos == 2) {
			this->is_playing = false;
			return;
		}

		break;
	}

}

gameround::~gameround() {

}