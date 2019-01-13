// C++ Yaniv Card Game
// Yonah Lawrence
// 8-11 January, 2019
// 

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

/* NUMBERS */
#define CARDS_AT_START 5 // cards each player is dealt
#define MIN_TO_CALL_YANIV 7 // minimum points to call Yaniv
#define ASSAF_PENALTY 30 // penalty for being Assaf-ed
#define EXTRA_ASSAF_PENALTIES 0 // additional penalties for each additional player who can Assaf you (usually 20 pts when applied)
#define POINTS_LIMIT 200 // Score that eliminates players
#define MIN_PLAYERS 2 // minimum number of players
#define MAX_PLAYERS 8 // maximum number of players

/* RULE VARIATIONS */
#define CAN_SLAPDOWN true // if you draw from the draw pile the same type of card you just played, you can slap down your drawn card
#define CAN_SWAP_JOKER true // if next player has the card that goes in place of the joker, they can swap for it
#define CAN_TAKE_FROM_MIDDLE_OF_SET true // if 3 or 4-of-a-kind played, you can choose any
#define REDUCTION_IS_HALF true // 50% reduction as opposed a 50-point reduction

// Clear screen with "CLS" on Windows or with "\n" in other compilers
#if defined(_WIN32) || defined(WIN32)
void clearScreen() { system("CLS"); }
#else
void clearScreen() { cout << string(50, '\n'); }
#endif

struct Player {
	bool AI = false;
	string name;
	vector <string> hand;
	int score = 0;
	int pointsInRound = 0;
	bool stillPlaying = true;
};

class Yaniv {
public:
	int numPlayers = 0;
	int numHuman = 0;
	int currentPlayer = 0;
	int remainingPlayers;
	vector <Player> players;
	vector <string> discardPile;
	vector <string> availableToTake;
	vector <string> nextAvailableToTake;
	vector <string> history;
	const vector <string> FULL_DECK = {
		"AC", "AH", "AS", "AD",
		"2C", "2H", "2S", "2D",
		"3C", "3H", "3S", "3D",
		"4C", "4H", "4S", "4D",
		"5C", "5H", "5S", "5D",
		"6C", "6H", "6S", "6D",
		"7C", "7H", "7S", "7D",
		"8C", "8H", "8S", "8D",
		"9C", "9H", "9S", "9D",
		"10C", "10H", "10S", "10D",
		"JC", "JH", "JS", "JD",
		"QC", "QH", "QS", "QD",
		"KC", "KH", "KS", "KD",
		"J", "J"
	};
	vector <string> deck = FULL_DECK;
	int playGame();
	void makePlayers();
	void dealCards(Player&, int = 1);
	void printVector(vector<string>);
	bool checkDiscards(Player&, string);
	string prevValue(string);
	string nextValue(string&);
	bool checkDraw(Player&, string);
	string getValue(string);
	char getSuit(string);
	int getOrder(vector <string>, int);
	void sortCards(vector <string>&);
	int pointsForCard(string);
	int countPoints(vector<string>);
	int callYaniv(Player&, int);
	void resetGame(int);
	void resetRound(int);
	void changeTurn();
	int countCardInVector(vector<string>, string, vector<string>&);
	vector <string> getBestDiscard(vector<string>, vector<string> = {""}, bool = true);
};

void trim(string &str);

int main() {
	// for random number generation
	srand((unsigned int)time(NULL));

	Yaniv game;

	clearScreen();
	cout << "Welcome to Yaniv!" << endl;
	cout << endl;
	cout << "## OBJECT AND SCORING ##" << endl;
	cout << "* Jokers are 0 points, Aces are 1, 2-10 are their value, and face cards are 10." << endl;
	cout << "* The object of the game is to be the player with the fewest points in their hand at the end of each round." << endl;
	cout << "* When a player has 7 or fewer points in their hand, they may end the round their next turn by calling \"Yaniv.\"" << endl;
	cout << "* If anyone has an equal or lower score in their hand when someone calls \"Yaniv,\" they call \"Assaf\" and win the round." << endl;
	cout << "* A 30-point penalty is added to the sum of the player who got \'Assaf-ed\'." << endl;
	cout << "* At the end of each round, the points each non-winner has in their hand are added to their cumulative score." << endl;
	cout << "* When a player reaches over 200 points, they are eliminated." << endl;
	cout << "* If a player's score lands on an exact multiple of 50 (ex. 50, 100, 150, 200), their score is divided in half." << endl;
	cout << endl;
	cout << "## PLAY ##" << endl;
	cout << "* Each player starts with 5 cards and the discard pile starts with one random card face-up." << endl;
	cout << "* On their turn, players discard a card or set and draw a card from either the draw pile or the last thrown cards." << endl;
	cout << "* A set can be either multiples (ex. 2, 3, or 4-of-a-kind) or a series of 3 or more consecutive cards of the same suit." << endl;
	cout << "* A Joker can be used as a wild-card in a series in place of a missing number (ex. 4H J 6H)." << endl;
	cout << "* When drawing a card from the last thrown cards, only the first or last card in the set can be drawn." << endl;
	cout << "* When a player has 7 or fewer points in their hand at the start of their turn, they may call \"Yaniv\" and end the round." << endl;
	cout << endl;
	cout << "## BONUS RULES ##" << endl;
	cout << "* Slapdown: If you play a card or multiples of a card and draw from the draw pile, if the card is of the same value, you may quickly slap down the card onto the discard pile." << endl;
	cout << "* If a player plays a series with a Joker, even if it is in the middle of a series, if the next player has the card that belongs in the place of Joker, they may swap the card for the Joker as their turn." << endl;
	cout << endl;

	game.makePlayers();

	// start playing game, continue new games until user does not want to play again
	while (true) {
		// start game of Yaniv, return winner of game to start next game
		int winner = game.playGame();

		// ask if user wants to start another game
		char response;
		while (true) {
			cout << "Do you want to play again? (Y/N): ";
			cin >> response;
			response = toupper(response);
			if (response == 'N') {
				return 0; // end program
			}
			else if (response == 'Y') {
				game.resetGame(winner); // reset scores, deck, and game
				cin.ignore();
				break;
			}
			else {
				cout << "Your response must be 'Y' or 'N'." << endl;
				cin.clear();
			}
		}
	}

	return 0;
}

int Yaniv::playGame() {
	int winner;
	char buffer[100];
	while (true) {
		if (history.size() >= players.size()) {
			history.erase(history.begin());
		}
		Player* activePlayer = &players[currentPlayer];
		if (!activePlayer->stillPlaying) { // if active player is out, skip to next player
			changeTurn();
		}
		else if (activePlayer->AI) { // if AI and still playing
			// clear screen print player number and wait for enter key
			clearScreen();
			cout << activePlayer->name << "'s turn." << endl;
			sortCards(activePlayer->hand);
			// if MIN_TO_CALL_YANIV or under, call Yaniv //
			int points = countPoints(activePlayer->hand);
			if (points <= MIN_TO_CALL_YANIV) {
				cout << activePlayer->name << " called Yaniv." << endl;
				winner = callYaniv(*activePlayer, points);
				if (remainingPlayers == 1) {
					cout << "The winner is " << players[winner].name << "!" << endl;
					return winner;
				}
				cout << "Press enter to start next round...";
				cin.getline(buffer, 100);
				resetRound(winner);
				/// check if certain that player has fewer///
			}
			else {
				string slapdown = "";
				// check if taking L or R will result in straight or set //
				// if yes, find best discard of non-saved cards (highest point straight/set) and take face up card //
				string discards = "";
				string drawnCard = "";
				vector <string> bestOfHand = getBestDiscard(activePlayer->hand);
				cout << "Your hand: "; // for debug
				printVector(activePlayer->hand); // for debug
				cout << "Top of discard pile: ";
				printVector(nextAvailableToTake);
				vector <string> bestWithTaking = getBestDiscard(activePlayer->hand, nextAvailableToTake);
				// if taking makes a difference and taking allows playing more than 1 card (or AI can take a Joker)
				vector <string> bestOfHandCards(bestOfHand.cbegin() + 1, bestOfHand.cend());
				vector <string>	bestWithTakingCards(bestWithTaking.cbegin() + 1, bestWithTaking.cend());
				if ((bestOfHandCards != bestWithTakingCards && bestWithTakingCards.size() > 1) || bestWithTaking.front() == "J"){
					// get cards not used in combination with face up card
					vector <string> cardsNotSaved = activePlayer->hand;
					for (size_t i = 0; i < bestWithTaking.size(); i++) {
						if (count(cardsNotSaved.begin(), cardsNotSaved.end(), bestWithTaking[i])) {
							cardsNotSaved.erase(find(cardsNotSaved.begin(), cardsNotSaved.end(), bestWithTaking[i]));
						}
					}
					// if AI has cards to save
					if (cardsNotSaved.size()) {
						bestOfHand = getBestDiscard(cardsNotSaved);
					}

					drawnCard = bestWithTaking.front();
					discardPile.erase(find(discardPile.begin(), discardPile.end(), bestWithTaking.front()));
					activePlayer->hand.push_back(bestWithTaking.front());
					sortCards(activePlayer->hand);
				}
				else {
					// check if drawing an available card will help AI on next turn
					bool drawnCardAlready = false;
					vector <string> savedCards = activePlayer->hand;
					for (size_t i = 1; i < bestOfHand.size(); i++)	{
						savedCards.erase(find(savedCards.begin(),savedCards.end(), bestOfHand[i]));
					}
					// if AI has cards to save
					if (savedCards.size()) {
						vector <string> bestOfSaved = getBestDiscard(savedCards);
						vector <string> bestOfSavedWithTaking = getBestDiscard(savedCards, nextAvailableToTake, false);
						vector <string> bestOfSavedCards(bestOfSaved.cbegin() + 1, bestOfSaved.cend());
						vector <string>	bestOfSavedWithTakingCards(bestOfSavedWithTaking.cbegin() + 1, bestOfSavedWithTaking.cend());
						// if taking makes a difference and taking allows playing more than 1 card (or AI can take a Joker)
						if ((bestOfSavedCards != bestOfSavedWithTakingCards && bestOfSavedWithTakingCards.size() > 1) || bestOfSavedWithTaking.front() == "J") {
							// draw card from available
							drawnCard = bestOfSavedWithTaking.front();
							discardPile.erase(find(discardPile.begin(), discardPile.end(), bestOfSavedWithTaking.front()));
							activePlayer->hand.push_back(bestOfSavedWithTaking.front());
							sortCards(activePlayer->hand);
							drawnCardAlready = true;
						}
					}

					if (!drawnCardAlready) {
						// if average of non-held/discarded cards is lower than left and right available discard, take from deck //
						// figure out average value of cards not in AI's hand or discard pile
						vector <string> unknowns = deck;
						for (size_t i = 0; i < players.size(); i++) {
							if (i != currentPlayer) {
								unknowns.insert(unknowns.end(), players[i].hand.begin(), players[i].hand.end());
							}
						}
						int sumOfUnknowns = 0;
						for (size_t i = 0; i < unknowns.size(); i++) {
							sumOfUnknowns += pointsForCard(unknowns[i]);
						}
						int averageOfUnknowns = sumOfUnknowns / unknowns.size();

						// take from draw pile if:
						// EITHER average of unknowns is less than the lower of available cards (left card is always smaller or equal to right card)
						// OR your hand is very low, gamble for a card lower than available
						int leftoverPts = countPoints(activePlayer->hand) - countPoints(bestOfHand);
						if ((averageOfUnknowns < pointsForCard(nextAvailableToTake[0])) ||
							(leftoverPts <= 7 && pointsForCard(nextAvailableToTake[0]) >= (8 - leftoverPts))) {
							// take from draw pile
							dealCards(*activePlayer);
							if (CAN_SLAPDOWN) {
								// check for slapdown
								drawnCard = activePlayer->hand.back();
								if (getValue(drawnCard) == getValue(bestOfHand[1]) && getValue(drawnCard) == getValue(bestOfHand.back()) && drawnCard != "J") {
									slapdown = drawnCard;
								}
							}
							// hide card from other players
							drawnCard = "from the draw pile";
						}
						// if not, take smaller of face up cards
						else {
							drawnCard = nextAvailableToTake[0];
							discardPile.erase(find(discardPile.begin(), discardPile.end(), nextAvailableToTake[0]));
							activePlayer->hand.push_back(nextAvailableToTake[0]); // take from discard
						}
					}
				}
				// discard best of held cards //
				cout << activePlayer->name << " discarded";
				for (size_t i = 1; i < bestOfHand.size(); i++) {
					cout << " " + bestOfHand[i];
					discards += bestOfHand[i] + " ";
				}
				trim(discards);
				if (!checkDiscards((*activePlayer), discards)) {
					cout << "ERROR IN DISCARD";
				}
				cout << "." << endl;

				cout << activePlayer->name << " picked up " << drawnCard << "." << endl;

				if (slapdown != "") {
					cout << "The " << slapdown << " that was drawn was slapped down!" << endl;
					activePlayer->hand.pop_back(); // remove last card from hand
					discardPile.push_back(slapdown); // add to discard pile
					nextAvailableToTake.push_back(slapdown); // add to end of next available to take
				}

				// put discarded cards into availableToTake for next player
				availableToTake = nextAvailableToTake;

				string turn = activePlayer->name + " discarded " + discards + ", picked " + drawnCard + ", and now has " + to_string(activePlayer->hand.size()) + (activePlayer->hand.size() == 1 ? " card." : " cards.");
				history.push_back(turn);

				cout << "Press enter to continue...";
				cin.getline(buffer, 100);
				changeTurn();
			}
		}
		else { // if Human and still playing

			// clear screen
			clearScreen();

			// hide cards if playing with others on-device
			if (numHuman > 1) {
				// print player number and wait for enter key, clear screen
				cout << activePlayer->name << "'s turn." << endl;
				cout << "Press enter to display hand...";
				cin.getline(buffer, 100);
				clearScreen();
			}

			// print current player, player's hand, and available cards
			cout << activePlayer->name << "'s turn." << endl;

			// inform player of round history
			for (size_t i = 0; i < history.size(); i++) {
				cout << history[i] << endl;
			}
			if (history.size()) { cout << endl; }

			cout << "Your hand: ";
			printVector(activePlayer->hand);
			cout << "Top of discard pile: ";
			printVector(availableToTake);

			// check if can call Yaniv
			bool calledYaniv = false;
			int points = countPoints(activePlayer->hand);
			if (points <= MIN_TO_CALL_YANIV) {
				char response;
				while (true) {
					cout << "Do you want to call Yaniv? (Y/N): ";
					cin >> response;
					response = toupper(response);
					if (response == 'N') {
						cin.ignore();
						break;
					}
					else if (response == 'Y') {
						calledYaniv = true;
						cin.ignore();
						break;
					}
					else {
						cout << "Your response must be 'Y' or 'N'." << endl;
						cin.clear();
					}
				}
			}

			// if called Yaniv
			if (calledYaniv) {
				cout << activePlayer->name << " called Yaniv." << endl;
				winner = callYaniv((*activePlayer), points);
				if (remainingPlayers == 1) {
					cout << "The winner is " << players[winner].name << "!" << endl;
					return winner;
				}
				cout << "Press enter to start next round...";
				cin.getline(buffer, 100);
				resetRound(winner);
			}
			else {
				// discard cards
				string discards;
				while (true) {
					cout << "Which cards do you want to discard? ";
					getline(cin, discards);
					trim(discards);
					transform(discards.begin(), discards.end(), discards.begin(), ::toupper);
					if (checkDiscards((*activePlayer), discards)) {
						cout << "Your hand: ";
						printVector(activePlayer->hand);
						break;
					}
				}

				// display available cards
				cout << "Top of discard pile: ";
				printVector(availableToTake);

				// choose card to draw
				string draw;
				while (true) {
					cout << "Which card do you want to draw (type 'D' for draw pile)? ";
					getline(cin, draw);
					trim(draw);
					transform(draw.begin(), draw.end(), draw.begin(), ::toupper);
					if (checkDraw(players[currentPlayer], draw)) {
						break;
					}
				}

				// put discarded cards into availableToTake for next player
				availableToTake = nextAvailableToTake;

				string turn = activePlayer->name + " discarded " + discards + ", picked " + (draw == "D" ? "from the draw pile" : draw) + ", and now has " + to_string(activePlayer->hand.size()) + (activePlayer->hand.size() == 1 ? " card." : " cards.");
				history.push_back(turn);

				cout << "Press enter to end turn...";
				cin.getline(buffer, 100);

				changeTurn();
			}
		}
	}
}

void Yaniv::makePlayers() {
	int numAI;
	while (true) {
		cout << "How many humans? ";
		cin >> numHuman;
		numPlayers = numHuman;
		if (cin.fail() || numPlayers < 0 || numPlayers > MAX_PLAYERS) {
			cout << "The number of players must be between " << MIN_PLAYERS << " and " << MAX_PLAYERS << "." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			cin.ignore();
			break;
		}
	}
	while (true) {
		cout << "How many AIs? ";
		cin >> numAI;
		numPlayers = numHuman + numAI;
		if (cin.fail() || numPlayers < MIN_PLAYERS || numPlayers > MAX_PLAYERS) {
			cout << "The number of players must be between " << MIN_PLAYERS << " and " << MAX_PLAYERS << "." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			cin.ignore();
			break;
		}
	}

	remainingPlayers = numPlayers;

	// shuffle cards for extra randomness
	random_shuffle(deck.begin(), deck.end());

	// make players array and deal 5 cards to each
	for (int i = 0; i < numPlayers; i++) {
		// add player to array
		Player player;
		players.push_back(player);

		// set player name
		while (true) {
			// request name for player
			string name;
			cout << "Enter name for Player " << (i + 1);
			if (i >= numHuman) { // set AI status of player to true
				players[i].AI = true;
				cout << " [AI]";
			}
			cout << ": ";
			getline(cin, name);
			trim(name);

			// check if name is valid (not taken and not blank)
			bool nameTaken = false;
			for (size_t j = 0; j < players.size() - 1; j++) {
				if (players[j].name == name) {
					nameTaken = true;
					break;
				}
			}
			if (nameTaken) {
				cout << "That name is already being used." << endl;
			}
			else if (name.length() == 0) {
				cout << "Name cannot be blank." << endl;
			}
			else {
				players[i].name = name;
				break;
			}
		}

		// deal and sort cards
		dealCards(players[i], CARDS_AT_START);
		sortCards(players[i].hand);
	}

	// make a card available for first player to take
	int cardNum = rand() % deck.size();
	discardPile.push_back(deck[cardNum]);
	availableToTake.push_back(deck[cardNum]);
	nextAvailableToTake = availableToTake;
	deck.erase(deck.begin() + cardNum);
}

void Yaniv::dealCards(Player &player, int numCards) {
	if (deck.size() <= 0) {
		// move discard pile back into deck, leave available cards in discard pile
		int cardsToTransfer = discardPile.size() - nextAvailableToTake.size();
		for (int i = 0; i < cardsToTransfer; i++) {
			deck.push_back(discardPile[0]);
			discardPile.erase(discardPile.begin());
		}
	}
	for (int i = 0; i < numCards; i++) {
		int cardNum = rand() % deck.size();
		player.hand.push_back(deck[cardNum]);
		deck.erase(deck.begin() + cardNum); // remove card from deck
	}
}

void Yaniv::printVector(vector<string> v) {
	for (size_t i = 0; i < v.size(); i++) {
		cout << v[i] << " ";
	}
	cout << endl;
}

bool Yaniv::checkDiscards(Player &player, string discards) {
	if (!discards.length()) {
		return false; // if discards is blank
	}
	vector <string> cardsToDiscard;
	vector <string> discardV;
	string currCard;
	for (size_t i = 0; i < discards.length() + 1; i++) {
		if (discards[i] == ' ' || discards[i] == '\0') {
			discardV.push_back(currCard);
			currCard = "";
		}
		else {
			currCard += discards[i];
		}
	}

	for (size_t i = 0; i < discardV.size(); i++) {
		if (discardV[i].length()) {
			if (!count(player.hand.begin(), player.hand.end(), discardV[i])) {
				cout << "You don't have " << discardV[i] << "." << endl;
				return false;
			}
			else {
				cardsToDiscard.push_back(discardV[i]);
			}
		}
	}

	// if tried putting in 2 jokers when player only has 1
	if (count(player.hand.begin(), player.hand.end(), "J") < count(cardsToDiscard.begin(), cardsToDiscard.end(), "J")) {
		cout << "You don't have " << count(cardsToDiscard.begin(), cardsToDiscard.end(), "J") << " Jokers." << endl;
		return false;
	}

	// check if all values are the same
	bool validDiscard = true;
	string firstCardValue = getValue(cardsToDiscard[0]);
	for (size_t i = 1; i < cardsToDiscard.size(); i++) {
		// if value on any remaining card is not the same as the first
		if (getValue(cardsToDiscard[i]) != firstCardValue) {
			validDiscard = false;
			break;
		}
	}

	// if not single, doubles, 3-of-a-kind, or 4-of-a-kind, check if it's a straight (3 or more cards)
	if (!validDiscard && cardsToDiscard.size() >= 3) {
		// check if straight of same suit
		string currCardValue;
		char firstCardSuit;
		size_t start;
		validDiscard = true;
		// Get suit and value of first card that's NOT a Joker
		for (size_t i = 0; i < cardsToDiscard.size(); i++) {
			if (cardsToDiscard[i] != "J") {
				currCardValue = getValue(cardsToDiscard[i]);
				firstCardSuit = getSuit(cardsToDiscard[i]);
				start = i + 1;
				break;
			}
		}
		for (size_t i = start; i < cardsToDiscard.size(); i++) {
			// if not a joker, check card
			if (cardsToDiscard[i] != "J") {
				// if suit on any remaining card is not the same as the first
				if (cardsToDiscard[i][cardsToDiscard[i].length() - 1] != firstCardSuit) {
					validDiscard = false;
					break;
				}
				// if value matches value after previous card's value (increment currCardValue to next one)
				if (getValue(cardsToDiscard[i]) != nextValue(currCardValue)) {
					validDiscard = false;
					break;
				}
			}
			else {
				// skip value that is replaced by Joker
				nextValue(currCardValue);
				// if next card is also Joker, skip a second time (ex. 7D J J 10D)
				if (i != cardsToDiscard.size() - 1) {
					if (cardsToDiscard[i + 1] == "J") {
						nextValue(currCardValue);
						i++;
					}
				}

			}
		}
	}

	// if valid single, doubles, 3-of-a-kind, or 4-of-a-kind, or straight
	if (validDiscard) {
		nextAvailableToTake.clear(); // clear available to take and refill with discards
		for (size_t i = 0; i < cardsToDiscard.size(); i++) {
			discardPile.push_back(discardV[i]);
			nextAvailableToTake.push_back(discardV[i]);
			player.hand.erase(find(player.hand.begin(), player.hand.end(), discardV[i]));
		}
		return true;
	}
	else {
		cout << "The selected cards can not be discarded together." << endl;
		return false;
	}
}

string Yaniv::prevValue(string value) {
	if (value == "J") { value = "10"; }
	else if (value == "Q") { value = "J"; }
	else if (value == "K") { value = "Q"; }
	else if (value == "A") { value = "--"; }
	else if (value == "10") { value = "9"; }
	else if (value.at(0) >= '2') { --value.at(0); }
	else if (value == "2") { value = "A"; }
	return value;
}

string Yaniv::nextValue(string &value) {
	if (value == "10") { value = "J"; }
	else if (value == "J") { value = "Q"; }
	else if (value == "Q") { value = "K"; }
	else if (value == "K") { value = "-"; }
	else if (value == "A") { value = "2"; }
	else if (value.at(0) <= '8') { ++value.at(0); }
	else if (value == "9") { value = "10"; }
	return value;
}

bool Yaniv::checkDraw(Player &player, string draw) {
	bool validDraw = false;
	if (draw.size() == 0) { return false; }
	if (draw.at(0) == 'D' && draw.length() == 1) {
		// take from draw pile
		dealCards(player);

		if (CAN_SLAPDOWN) {
			// check for slapdown
			string drawnCard = player.hand.back();
			if (getValue(drawnCard) == getValue(nextAvailableToTake.front()) && getValue(drawnCard) == getValue(nextAvailableToTake.back()) && drawnCard != "J") {
				// ask if user wants to do slapdown
				char response;
				while (true) {
					cout << "Do you want to slap down the " << drawnCard << " you drew? (Y/N): ";
					cin >> response;
					response = toupper(response);
					if (response == 'N') {
						cin.ignore();
						break;
					}
					else if (response == 'Y') {
						cout << "The " << drawnCard << " you drew was slapped down!" << endl;
						player.hand.pop_back(); // remove last card from hand
						discardPile.push_back(drawnCard); // add to discard pile
						nextAvailableToTake.push_back(drawnCard); // add to end of next available to take
						cin.ignore();
						break;
					}
					else {
						cout << "Your response must be 'Y' or 'N'." << endl;
						cin.ignore();
						cin.clear();
					}
				}
			}
		}

		// sort and display hand
		sortCards(player.hand);
		cout << "Your hand: ";
		printVector(player.hand);
		return true;
	}
	// if taking the first or last card that was played
	else if (draw == availableToTake.front() || draw == availableToTake.back()) {
		validDraw = true;
	}
	// if the cards down are multiples of the same card
	else if (availableToTake.front() == availableToTake.back() && availableToTake.front() != "J") {
		// if can take any card when multiples are played, and requested card is available
		if (CAN_TAKE_FROM_MIDDLE_OF_SET && count(availableToTake.begin(), availableToTake.end(), draw)) {
			validDraw = true;
		}
	}
	// if the cards are a straight, you put 1 card down and are requesting a joker which exists in straight
	else if (CAN_SWAP_JOKER && nextAvailableToTake.size() == 1 && draw == "J" && count(availableToTake.begin(), availableToTake.end(), "J")) {
		// if card played is the joker's replacement
		int JokerPos = find(availableToTake.begin(), availableToTake.end(), "J") - availableToTake.begin();
		string cardPlayed = nextAvailableToTake.front(), nextCard = availableToTake[(JokerPos + 1)], prevCard = availableToTake[(JokerPos - 1)];
		string cardPlayedValue = getValue(cardPlayed), nextCardValue = getValue(nextCard), prevCardValue = getValue(prevCard);
		char cardPlayedSuit = getSuit(cardPlayed), nextCardSuit = getSuit(nextCard), prevCardSuit = getSuit(prevCard);
		// if Joker is not the last card or first card
		if (JokerPos != availableToTake.size() - 1 && JokerPos) {
			// if next/prev card has same suit or is J AND next card has next value AND prev card has prev value
			if ((nextCardSuit == cardPlayedSuit || nextCardSuit == 'J') &&
				(prevCardSuit == cardPlayedSuit || prevCardSuit == 'J') &&
				(prevCardValue == prevValue(cardPlayedValue)) &&
				(nextCardValue == nextValue(cardPlayedValue))) {
				validDraw = true;
			}
		}
	}
	if (validDraw) {
		discardPile.erase(find(discardPile.begin(), discardPile.end(), draw));
		player.hand.push_back(draw);
		sortCards(player.hand);
		cout << "Your hand: ";
		printVector(player.hand);
		return true;
	}
	// if did not return true
	cout << "You can't take that card." << endl;
	return false;
}

string Yaniv::getValue(string card) {
	return card.substr(0, card.length() - 1);
}

char Yaniv::getSuit(string card) {
	return card.at(card.length() - 1);
}

int Yaniv::getOrder(vector <string> hand, int i) {
	string value = getValue(hand[i]);
	if (value == "") { return 0; }
	else if (value == "10") { return 10; }
	else if (value == "J") { return 11; }
	else if (value == "Q") { return 12; }
	else if (value == "K") { return 13; }
	else if (value == "A") { return 1; }
	else if (value.at(0) <= '9') {
		return value.at(0) - '0';
	}
	return -1;
}

void Yaniv::sortCards(vector <string> &hand) {
	size_t i;
	int j, key;
	string keyStr;
	for (i = 1; i < hand.size(); i++) {
		keyStr = hand[i];
		key = getOrder(hand, i); //arr[i];
		j = i - 1;
		while (j >= 0 && getOrder(hand, j) > key) { // arr[j]
			hand[j + 1] = hand[j]; // arr[j]
			j = j - 1;
		}
		hand[j + 1] = keyStr;
	}
}

int Yaniv::pointsForCard(string card) {
	string value = getValue(card);
	if (value == "") { // Joker is only 1 char, so substring will be "". (Joker adds 0)
		return 0;
	}
	if (value == "10" || value == "J" || value == "Q" || value == "K") {
		return 10;
	}
	else if (value == "A") {
		return 1;
	}
	else if (value.at(0) <= '9') {
		return (value.at(0) - '0');
	}
	return -1; // should never happen
}

int Yaniv::countPoints(vector <string> hand) {
	int sum = 0;
	for (size_t i = 0; i < hand.size(); i++) {
		sum += pointsForCard(hand[i]);
	}
	return sum;
}

int Yaniv::callYaniv(Player &activePlayer, int activePlayerPoints) {
	int lowestPts = activePlayerPoints;
	activePlayer.pointsInRound = activePlayerPoints;
	int winner = currentPlayer;
	vector <int> winners = {};

	cout << endl << activePlayer.name << "'s hand: ";
	cout << "(" << activePlayerPoints << " point" << (activePlayerPoints != 1 ? "s) " : ") ");
	printVector(activePlayer.hand);

	for (size_t i = 0; i < players.size(); i++) {
		if ((i != currentPlayer) && (players[i].stillPlaying)) {
			players[i].pointsInRound = countPoints(players[i].hand);
			cout << players[i].name << "'s hand: ";
			cout << "(" << players[i].pointsInRound << " point" << (activePlayerPoints != 1 ? "s) " : ") ");
			printVector(players[i].hand);
			if (players[i].pointsInRound <= lowestPts) {
				lowestPts = players[i].pointsInRound;
				winner = i;
			}
		}
	}

	players[winner].pointsInRound = 0; // winner gets 0 for round
	winners.push_back(winner); // add winner to potential players who can start next round (if only winner, they will start next round)

	// if active player was Assaf-ed, print Assaf-ers and deduct penalty
	if (winner != currentPlayer) {
		cout << players[currentPlayer].name << " was Assaf-ed by " << players[winner].name;
		activePlayer.score += ASSAF_PENALTY; // penalty

		// check if assaf was a tie
		for (size_t i = 0; i < players.size(); i++) {
			if (i != currentPlayer && i != winner && players[i].stillPlaying) { // if not caller of yaniv or declared winner with assaf
				// if playing with extra penalties, usually 20 extra points are deducted for additional Assafs
				if (players[i].pointsInRound <= activePlayerPoints) {
					cout << " and " << players[i].name; // add name to assaf list
					activePlayer.score += EXTRA_ASSAF_PENALTIES; // default is 0, but some variations add 20 points here
				}
				if (players[i].pointsInRound == lowestPts) { // if player has same amount as winner with assaf
					players[i].pointsInRound = 0; // also gets 0 for round
					winners.push_back(i); // give potential for this player to be first player next round
				}
			}
		}

		cout << "." << endl;
	}

	// print winners
	cout << players[winners.front()].name << " ";
	for (size_t i = 1; i < winners.size(); i++)
		cout << "and " << players[winners[i]].name << " ";
	cout << "won the round." << endl;

	cout << endl << "SCOREBOARD:" << endl;

	for (size_t i = 0; i < players.size(); i++) {
		bool scoreHalved = false;
		players[i].score += players[i].pointsInRound;
		// if score is divisible by 50 and score increased (i.e. 50, 100, 150, 200), halve score
		if (players[i].pointsInRound > 0 && players[i].score % 50 == 0) {
			if (REDUCTION_IS_HALF) { players[i].score /= 2; } // if true, halve score
			else { players[i].score -= 50; } // if false, deduct 50
			scoreHalved = true;
		}
		// print player's name and score
		cout << players[i].name << ": " << players[i].score << " point" << (players[i].score != 1 ? "s" : "");
		// add message if score was halved
		if (scoreHalved) {
			cout << " -- " << players[i].name << (REDUCTION_IS_HALF ? "'s score was halved!" : "'s score was deducted by 50!");
		}
		// knock players out of round if hit max
		if (players[i].score > POINTS_LIMIT) {
			cout << " -- OUT";
			if (players[i].stillPlaying) {
				players[i].pointsInRound = 0;
				players[i].stillPlaying = false; // will be skipped in future rounds
				--remainingPlayers; // decrement remaining players
			}
		}
		cout << endl;
	}
	cout << endl;

	return winners[rand() % winners.size()]; // return random one of winners if multiple players assaf-ed
}

void Yaniv::resetGame(int winner) {
	remainingPlayers = numPlayers;
	for (size_t i = 0; i < players.size(); i++) {
		players[i].score = 0;
		players[i].stillPlaying = true;
	}
	resetRound(winner);
}

void Yaniv::resetRound(int winner) {
	deck = FULL_DECK;
	random_shuffle(deck.begin(), deck.end());
	currentPlayer = winner;
	discardPile.clear();
	availableToTake.clear();
	nextAvailableToTake.clear();
	history.clear();

	// clear hands and deal cards
	for (size_t i = 0; i < players.size(); i++) {
		players[i].hand.clear();
		dealCards(players[i], CARDS_AT_START);
		sortCards(players[i].hand);
	}

	// make a card available for first player to take
	int cardNum = rand() % deck.size();
	discardPile.push_back(deck[cardNum]);
	availableToTake.push_back(deck[cardNum]);
	nextAvailableToTake = availableToTake;
	deck.erase(deck.begin() + cardNum);
}

void Yaniv::changeTurn() {
	if (currentPlayer == numPlayers - 1) {
		currentPlayer = 0;
	}
	else {
		++currentPlayer;
	}
}

int Yaniv::countCardInVector(vector<string> v, string c, vector<string> &discard) {
	int count = 0, length = v.size();
	for (int i = 0; i < length; i++) {
		string value = getValue(v[i]);
		if (value == c) {
			discard.push_back(v[i]);
			++count;
		}
	}
	return count;
}

vector <string> Yaniv::getBestDiscard(vector<string> hand, vector<string> availableCards, bool takingFromDiscard) {
	vector <string> discard;
	int bestDiscardPts = pointsForCard(hand.back());
	vector <string> bestDiscard = { hand.back() };
	string draw = "";
	string bestDraw = draw;
	int numAvailableCards = availableCards.size();
	if (availableCards.front() == "") {
		takingFromDiscard = false;
	}

	bool ableToTakeFromMiddle = false;
	// if the cards down are multiples of the same card
	// and if can take any card when multiples are played
	if (CAN_TAKE_FROM_MIDDLE_OF_SET && availableCards.front() == availableCards.back() && availableCards.front() != "J") {
		ableToTakeFromMiddle = true;
	}

	int j = 0;

	// do twice if available cards
	while (j < numAvailableCards) {
		// if checking available cards
		if (availableCards.front() != "") {
			if (!ableToTakeFromMiddle && j == 1) { // if done with 0 and can't take from middle
				j = availableCards.size() - 1; // skip to last card of available cards
			}
			draw = availableCards[j]; // card to draw is j
			hand.push_back(draw); // add draw to temporary hand
			sortCards(hand);
		}

		// check for multiples
		const string cardValues[] = { "","A","2","3","4","5","6","7","8","9","10","J","Q","K" };
		const int cardValuesPts[] = { 0,1,2,3,4,5,6,7,8,9,10,10,10,10 };
		for (int k = 0; k < 14; k++) {
			vector <string> savedCards = hand;
			discard.clear();
			int pointsForAllOfValue = countCardInVector(hand, cardValues[k], discard) * cardValuesPts[k];
			if (draw == "J") { pointsForAllOfValue += 99; } // if can draw a joker, always do it
			if (pointsForAllOfValue > bestDiscardPts) {
				for (size_t i = 0; i < discard.size(); i++)
					savedCards.erase(find(savedCards.begin(), savedCards.end(), discard[i]));
				if (savedCards.size() == 0) { savedCards.push_back(""); }
				// if taking a card, must play more than 1 card, otherwise, 1 card is fine
				// if picking an available card, then highest card after discarding this set should not be a Joker
				if ((discard.size() > 1 && savedCards.back() != "J") || (!takingFromDiscard)) {
					bestDiscardPts = pointsForAllOfValue;
					bestDiscard = discard;
					bestDraw = draw;
				}
			}
		}

		// check for series
		int numJokers = count(hand.begin(), hand.end(), "J");
		vector <string> savedCards;
		for (size_t k = 0; k < hand.size(); k++) {
			string firstCardValue;
			char firstCardSuit;
			int unusedJokers = numJokers;
			savedCards = hand;
			discard.clear();
			// Get suit and value of first card that's NOT a Joker
			for (size_t i = k; i < hand.size(); i++) {
				if (hand[i] != "J") {
					firstCardValue = getValue(hand[i]);
					firstCardSuit = getSuit(hand[i]);
					discard.push_back(hand[i]);
					savedCards.erase(savedCards.begin()+i);
					k = i;
					break;
				}
			}
			if (!discard.size()) { break; }
			// check remaining cards if can make a series
			for (size_t l = k + 1; l <= hand.size() + unusedJokers; l++) {
				string nextVal = nextValue(firstCardValue);
				string nextCard = nextVal + firstCardSuit;
				// if hand contains next card needed in series
				if (count(hand.begin() + k, hand.end(), nextCard)) {
					discard.push_back(nextCard);
					savedCards.erase(find(savedCards.begin(), savedCards.end(), nextCard));
				}
				// if don't have the right card and can use joker (and last card was not a king)
				else if (unusedJokers > 0 && getValue(nextCard) != "-") {
					--unusedJokers;
					discard.push_back("J");
					savedCards.erase(savedCards.begin());
				}
				// if last card was a king, have a joker, and don't already have 3 cards to discard
				else if (unusedJokers > 0 && getValue(nextCard) != "-" && discard.size() < 3) {
					--unusedJokers;
					discard.insert(discard.begin(), "J"); // put joker at front
					savedCards.erase(savedCards.begin());
					break;
				}
				else {
					break; // can't continue series
				}
			}
			while (discard.back() == "J") {
				// if saved cards are 3 or fewer points and card before leading joker is not a joker and joker is needed as 3rd card
				// i.e. if hand is low and using only 1 joker to get rid of a couple extra cards, keep joker on end of series
				if (countPoints(savedCards) <= MIN_TO_CALL_YANIV - 4 && discard[discard.size() - 2] != "J" && discard.size() <= 3) {
					break;
				}
				discard.pop_back(); // don't end series with a joker
				savedCards.insert(savedCards.begin(), "J");
			}
			if (savedCards.size() == 0) { savedCards.push_back(""); }
			// if taking to make a series and highest card you can discard is a joker
			if ((savedCards.back() == "J" && takingFromDiscard)) {
				discard.clear(); // don't discard joker to make series
			}
			if (discard.size() >= 3) {
				int pointsForSeries = countPoints(discard);
				if (draw == "J") { pointsForSeries += 99; } // if can draw a joker, always do it
				if (pointsForSeries > bestDiscardPts) {
					bestDiscardPts = pointsForSeries;
					bestDiscard = discard;
					bestDraw = draw;
				}
			}
		}

		// if not checking available cards break after 1 pass
		if (availableCards.front() == "") {
			break;
		}

		// remove additional card to check with 2nd available card only
		hand.erase(find(hand.begin(), hand.end(), draw));

		++j;
	}

	bestDiscard.insert(bestDiscard.begin(), bestDraw);
	return bestDiscard;
}

void trim(string &str) {
	// Removes all spaces from the beginning of the string
	while (str.size() && isspace(str.front()))
		str.erase(str.begin());
	// Remove all spaces from the end of the string.
	while (str.size() && isspace(str.back()))
		str.pop_back();
}
