# CPP-Yaniv

Yaniv Card Game written in C++ that is played in the console.

You can choose 2 to 8 humans or AIs to play against each other on the same device.

Demo: https://repl.it/repls/DarkgrayLankyDominspector

## How To Play

#### Object and Scoring
* Jokers are 0 points, Aces are 1, 2-10 are their value, and face cards are 10.
* The object of the game is to be the player with the fewest points in their hand at the end of each round.
* When a player has 7 or fewer points in their hand, they may end the round their next turn by calling "Yaniv."
* If anyone has an equal or lower score in their hand when someone calls "Yaniv," they call "Assaf" and win the round.
* A 30-point penalty is added to the sum of the player who got 'Assaf-ed'.
* At the end of each round, the points each non-winner has in their hand are added to their cumulative score.
* When a player reaches over 200 points, they are eliminated.
* If a player's score lands on an exact multiple of 50 (ex. 50, 100, 150, 200), their score is divided in half.

#### Play
* Each player starts with 5 cards and the discard pile starts with one random card face-up.
* On their turn, players discard a card or set and draw a card from either the draw pile or the last thrown cards.
* A set can be either multiples (ex. 2, 3, or 4-of-a-kind) or a series of 3 or more consecutive cards of the same suit.
* A Joker can be used as a wild-card in a series in place of a missing number (ex. 4H J 6H).
* When drawing a card from the last thrown cards, only the first or last card in the set can be drawn.
* When a player has 7 or fewer points in their hand at the start of their turn, they may call "Yaniv" and end the round.

#### Bonus Rules
* Slapdown: If you play a card or multiples of a card and draw from the draw pile, if the card is of the same value, you may quickly slap down the card onto the discard pile.
* If a player plays a series with a Joker, even if it is in the middle of a series, if the next player has the card that belongs in the place of Joker, they may swap the card for the Joker as their turn.

#### Variations
Variations can be set by changing the defined macros at the beginning of the code
* Slapdown
* Joker Swap
* 50-point reduction instead of 50% reduction on multiples of 50
* Different number of points required to call Yaniv (ex. 5, 7, 9, 11)
* Extra 20-point penalties to caller of Yaniv for every additional player who can call Assaf
* Allow picking up any card in a set of 3-of-a-kind or 4-of-a-kind and not just the first or last (the rule still applies to series)

## What I plan to do next:

#### Improvements to AI:
* Allow AI to find a series in its hand or pick up a card to make a series (currently AI only plays multiples)
* Assaf suspicion - Prevent AI from calling Yaniv if it suspects a player may have fewer points based on the cards they have picked up publicly and the number of cards in their hand
    
#### Improvents to provided text
* Before a person's turn, show cards drawn and discarded in the previous turn by each of the other players
* Show number of cards each player holds

#### Features to possibly add
* Allow users to choose the variation of the rules they want while running the program
* Allow user to undo their discard before they draw their card

#### Design improvements
* Form the shape of cards when printing instead of printing as text
