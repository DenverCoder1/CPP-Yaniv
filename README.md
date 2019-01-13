# CPP-Yaniv

Yaniv Card Game written in C++ that is played in the console.

You can choose 2 to 8 humans or AIs to play against each other on the same device.

Demo: https://repl.it/repls/DarkgrayLankyDominspector

## How To Play

### Object and Scoring
* Jokers are 0 points, Aces are 1, 2-10 are their value, and face cards are 10.
* The object of the game is to be the player with the fewest points in their hand at the end of each round.
* When a player has 7 or fewer points in their hand, they may end the round their next turn by calling "Yaniv."
* If anyone has an equal or lower score in their hand when someone calls "Yaniv," they call "Assaf" and win the round.
* A 30-point penalty is added to the sum of the player who got 'Assaf-ed'.
* At the end of each round, the points each non-winner has in their hand are added to their cumulative score.
* When a player reaches over 200 points, they are eliminated.
* If a player's score lands on an exact multiple of 50 (ex. 50, 100, 150, 200), their score is divided in half.

### Play
* Each player starts with 5 cards and the discard pile starts with one random card face-up.
* On their turn, players discard a card or set and draw a card from either the draw pile or the last thrown cards.
* A set can be either multiples (ex. 2, 3, or 4-of-a-kind) or a series of 3 or more consecutive cards of the same suit.
* A Joker can be used as a wild-card in a series in place of a missing number (ex. 4H J 6H).
* When drawing a card from the last thrown cards, only the first or last card in the set can be drawn.
* When a player has 7 or fewer points in their hand at the start of their turn, they may call "Yaniv" and end the round.
* The winner of the round gets 0 points added to their score and they start the next round.

### Bonus Rules
* Slapdown: If you play a card or multiples of a card and draw from the draw pile, if the card is of the same value, you may quickly slap down the card onto the discard pile.
* If a player plays a series with a Joker, even if it is in the middle of a series, if the next player has the card that belongs in the place of Joker, they may swap the card for the Joker as their turn.

### Optional Variations
* Choose to play with or without the 'Slap-down' rule
* Choose to allow or disallow players from swapping the corresponding card for a joker (even in the middle of a series)
* Choose to play with 50-point reductions instead of 50% reductions on multiples of 50
* Set a different number of points required to call Yaniv (ex. 5, 7, 9, 11)
* Start with a different number of cards
* Set the limit at which players are eliminated
* Set the penalty amount for getting Assaf-ed
* Set extra penalties for getting Assaf-ed by more than one player
* Allow picking up any card in a set of 3 or 4-of-a-kind and not just the first or last (the rule still applies to series)

## Features

* Play with 2 to 8 players being all human, all AI, or a combination of both.
* Set the names for each player
* You will be asked at the start of the game which rule variations you want to play with.
* As long as you have not yet chosen a card to draw you may type `undo` to re-select the cards you want to discard.
* The history of the previous turn is displayed so everyone knows what cards were discarded, drawn from the discard pile, and how many cards everyone has left.
* Illegal discards and draws are not playable and an error message will be displayed.
* Cards will always be displayed in sorted order with jokers on the left followed by A-K.

## What I plan to do next:

### Design improvements
* Form the shape of cards when printing instead of printing as text
