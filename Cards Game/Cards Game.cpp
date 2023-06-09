// Cards Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<iostream>
#include<vector>
#include<cstdlib>
#include<conio.h>
using namespace std;

enum Rank { ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
enum Suit { DIAMOND, SPADE, CLUB, HEART };

class Card {
protected:
    const int num_of_suits = 4;
    const int num_of_ranks = 13;

    Rank rank;
    Suit suit;

public:
    void Print_Card(const Card&);
};
void Card::Print_Card(const Card& card) {
    switch (card.rank)
    {
    case 0:
        cout << "ACE of ";
        break;
    case 1:
        cout << "2 of ";
        break;
    case 2:
        cout << "3 of ";
        break;
    case 3:
        cout << "4 of ";
        break;
    case 4:
        cout << "5 of ";
        break;
    case 5:
        cout << "6 of ";
        break;
    case 6:
        cout << "7 of ";
        break;
    case 7:
        cout << "8 of ";
        break;
    case 8:
        cout << "9 of ";
        break;
    case 9:
        cout << "10 of ";
        break;
    case 10:
        cout << "JACK of ";
        break;
    case 11:
        cout << "QUEEN of ";
        break;
    case 12:
        cout << "KING of ";
        break;
    }
    switch (card.suit)
    {
    case 0:
        cout << "DIAMOND" << endl;
        break;
    case 1:
        cout << "SPADE" << endl;
        break;
    case 2:
        cout << "CLUB" << endl;
        break;
    case 3:
        cout << "HEART" << endl;
        break;
    }

}


class Deck : public Card{
public:
    vector<Card> cards;
    string card_back;
    static int max_deck_size; //52 total cards

    void initializeDeck();
    void print_deck();
    void shuffleDeck();
};
int Deck :: max_deck_size = 52;

void Deck::initializeDeck() {

    Card card;
    for (int suit = 0; suit < num_of_suits; suit++)
    {
        for (int rank = 0; rank < num_of_ranks; rank++)
        {
            this->suit = Suit(suit); //type conversion bw number and enum (for eg. suit = 0 then it assigns to diamond
            this->rank = Rank(rank);
            cards.push_back(card);
        }
    }

}
void Deck::shuffleDeck()
{
    Deck Shuffled;
    while (!cards.empty())
    {
        size_t rand_index = rand() % cards.size();
        Shuffled.cards.push_back(cards[rand_index]);
        cards.erase(cards.begin() + rand_index);
    }
    cards = Shuffled.cards;
    card_back = Shuffled.card_back;
}
void Deck::print_deck() {
    for (Card c : cards)
    {
        Print_Card(c);
    }
}


class Game : public Deck{
protected:
    vector<Player> players;
    Deck deck;
    int num_players = 2;
    int num_cards_per_hand = 7;
public:

    bool deal_cards(); // 1
    void print_hand(const vector<Card>&); //2
    void initialize(); // 3
    void addPlayer(); // 4
    void print_game();
    void play_game(); // 5
    void display_state(size_t); // 6    
    int get_number_of_players() 
    {
        return num_players;
    }
        
};
void Game :: initialize()
{
    deck.initializeDeck();
    deck.shuffleDeck();
    addPlayer();
    
}
void Game :: addPlayer()
{
    for (int player = 0; player < num_players; player++)
    {
        Player new_player;
        players.push_back(new_player);
    }
}
bool Game :: deal_cards()
{
    if (deck.max_deck_size < num_players * num_cards_per_hand)
    {
        return false;
    }

    for (int i = 0; i < num_cards_per_hand; i++)
    {
        for (int player = 0; player < num_players; player++)
        {
            players[player].hand.push_back(deck.cards[0]);
            deck.cards.erase(deck.cards.begin());
        }

    }
    return true;
}
void Game :: print_game()
{
    for (int player = 0; player < num_players; player++)
    {
        print_hand(players[player].hand);
        cout << endl << endl;
    }
    
    deck.print_deck();
}
void Game :: display_state(size_t player)
{
    cout << "Player 1 Hand: " << endl;
    print_hand(players[player].hand);
    cout << "Player 1 Score: " << players[player].score << endl;

    cout << "Player 2 Hand: " << endl;
    size_t opponent = (player + 1) % num_players;
    print_hand(players[opponent].hand);
    cout << "Player 2 Score: " << players[opponent].score << endl;
}
void Game :: play_game()
{
    initialize();
    deal_cards();

    bool game_over = false;
    size_t player = 0;
    while (!game_over)
    {
        
        display_state(player);
        
        Rank rank = players[player].get_rank();

        size_t next_player = (player + 1) % num_players;

        //if (isMatch(players[next_player].hand, rank))
        if (players[next_player].isMatch(rank))                 // Next player cards matches our card ranks
        {
            //take_card(player, rank);
            players[player].take_card(players[next_player].hand, rank);
        }
        else
        {
            cout << "Go Fish..." << endl;
            // draw_card(player);
            players[player].draw_card(deck);
        }
        
        players[player].check_for_score();

        if (players[player].hand.empty())
        {
            game_over = true;
        }
        player = next_player;

        cout << "Game Over" << endl;
        for (size_t player = 0; player < players.size(); player++)
        {
            cout << "Player " << player << "'s score is " << players[player].score << endl;
        }
    }
}

class Player : public Deck {
    vector<Card> hand;
    string name;
    int score = 0;

    void print_hand(const vector<Card>&); // 1
    Rank getRank(const vector<Card>& hand); // 2
    bool isMatch(const vector<Card>&, Rank); // 3
    void take_card(size_t, Rank); // 4
    void draw_card(size_t); // 5
    void check_for_score(); // 6
};

void Player::print_hand()
{
    for (Card c : hand)
    {
        Print_Card(c);
    }
}
Rank Player::getRank()
{
    bool valid = false;
    int rank;
    while (!valid)
    {
        cout << "Enter a rank: ";
        cin >> rank;

        for (Card c : hand)
        {
            if (c.rank == Rank(rank))
            {
                valid = true;
            }
        }
    }
    return Rank(rank);
}
void Player::draw_card(size_t i)                           // *********************************************************** //
{
    if (cards.empty())
    {
        return;
    }
    else
    {
        hand.push_back(cards[0]);
        cards.erase(cards.begin());
    }
}
bool Player::isMatch(Rank rank)
{
    bool match = false;
    for (Card c : hand)
    {
        if (c.rank == rank)
        {
            match = true;
        }
    }
    return match;
}
void Player::take_card(vector<Card>&, Rank rank_choosed)
{
    size_t next_player = (current_player + 1) % Game::get_number_of_players();        // *********************************************************** //
    bool card_taken = false;
    size_t card_index = 0;

    while (!card_taken)
    {
        Card current_card = players[next_player].hand[card_index];
        if (current_card.rank == rank_choosed)
        {
            players[current_player].hand.push_back(current_card);
            players[next_player].hand.erase(players[next_player].hand.begin() + card_index);

            card_taken = true;
        }
        card_index++;
    }
}
void Player::check_for_score()
{
    vector<bool> marked(hand.size(), false); // here hand.size refers to vector size and false refers to initial values.

    for (size_t i = 0; i < hand.size() - 1; i++) // Comparison between two cards
                                                 // used -1 because if there is only one card left we dont have to compare it
    {
        for (size_t j = i + 1; j < hand.size() - 1; j++)
        {
            if (!marked[i] && !marked[j])
            {
                if (hand[i].rand == hand[j].rank)
                {
                    marked[i] = true;
                    marked[j] = true;
                    score++;
                }
            }
        }
    }

    size_t card = 0;
    while (!hand.empty() && card < marked.size())
    {
        if (marked[card])
        {
            hand.erase(hand.begin() + card);
            marked.erase(marked.begin() + card);
        }
        else
        {
            card++;
        }
    }
}


int main()
{
    Game game;
    game.print_game();
    game.play_game();

}





