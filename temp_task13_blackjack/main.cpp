#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

/**
 * Here are the rules for our version of Blackjack:
 *
 * The dealer gets one card to start (in real life, the dealer gets two, but one is face down so it doesn’t matter at this point).
 * The player gets two cards to start.
 * The player goes first.
 * A player can repeatedly “hit” or “stand”.
 * If the player “stands”, their turn is over, and their score is calculated based on the cards they have been dealt.
 * If the player “hits”, they get another card and the value of that card is added to their total score.
 * An ace normally counts as a 1 or an 11 (whichever is better for the total score). For simplicity, we’ll count it as an 11 here.
 * If the player goes over a score of 21, they bust and lose immediately.
 * When the player is done, it is the dealer’s turn.
 * The dealer repeatedly draws until they reach a score of 17 or more, at which point they must stop drawing.
 * If the dealer goes over a score of 21, they bust and the player wins immediately.
 * Otherwise, if the player has a higher score than the dealer, the player wins. Otherwise, the player loses (we’ll consider ties as dealer wins for simplicity).
 *
 */

enum class CardRank {
    ace,
    king,
    queen,
    jack,
    ten,
    nine,
    eight,
    seven,
    six,
    five,
    four,
    three,
    two,
    allRanks
};

enum class Suits {
    clubs,
    diamonds,
    hearts,
    spades,
    allSuits
};


std::string get_card_rank_name(const CardRank& rank) {
    switch (rank) {
    case CardRank::ace:
        return "A";
    case CardRank::king:
        return "K";
    case CardRank::queen:
        return "Q";
    case CardRank::jack:
        return "J";
    case CardRank::ten:
        return "T";
    case CardRank::nine:
        return "9";
    case CardRank::eight:
        return "8";
    case CardRank::seven:
        return "7";
    case CardRank::six:
        return "6";
    case CardRank::five:
        return "5";
    case CardRank::four:
        return "4";
    case CardRank::three:
        return "3";
    case CardRank::two:
        return "2";
    default:
        return "unknown rank";
    }
}

std::string get_card_suit_name(const Suits& suit) {
    switch (suit) {
    case Suits::clubs:
        return "C";
    case Suits::diamonds:
        return "D";
    case Suits::hearts:
        return "H";
    case Suits::spades:
        return "S";
    default:
        return "unknown suit";
    }
}

int get_rank_score(const CardRank rank) {
    switch (rank) {
    case CardRank::ace:
        return 11; // could be 1 or 11 (depend on what you need more)
    case CardRank::king:
        return 10;
    case CardRank::queen:
        return 10;
    case CardRank::jack:
        return 10;
    case CardRank::ten:
        return 10;
    case CardRank::nine:
        return 9;
    case CardRank::eight:
        return 8;
    case CardRank::seven:
        return 7;
    case CardRank::six:
        return 6;
    case CardRank::five:
        return 5;
    case CardRank::four:
        return 4;
    case CardRank::three:
        return 3;
    case CardRank::two:
        return 2;
    default:
        return 0;
    }
}


struct Card {
    CardRank rank {};
    Suits suit {};

    friend std::ostream& operator<<(std::ostream& out, const Card &card) {
        out << get_card_rank_name(card.rank) << get_card_suit_name(card.suit);
        return out;
    }
};


class Deck {
private:
    std::vector<Card> cards {};
    int current_card_index {-1};
public:
    Deck(){
        for (int i = 0; i < static_cast<int>(Suits::allSuits); ++i) {
            Suits suit = static_cast<Suits>(i);
            for (int j = 0; j < static_cast<int>(CardRank::allRanks); ++j) {
                CardRank rank = static_cast<CardRank>(j);
                cards.push_back(Card { rank, suit });
            }
        }
    }

    Card& dealCard() {
        current_card_index++;

        assert(current_card_index != 52 && "Deck::dealCard ran out of cards");
        return cards[current_card_index];
    }

    void shuffle() {
        std::random_device rd {};
        std::seed_seq sq {rd(),rd(),rd(),rd(),rd(),rd(),rd(),rd()}; // наповнюємо випадковими числами seed sequens
        std::mt19937 mt(sq); // ініціалізуємо Mersenne Twister

        std::shuffle(cards.begin(), cards.end(), mt);
    }

};


struct Player {
    int score {};

    void add_score(const CardRank rank) {
        if (rank == CardRank::ace) {
            const int temp_score_1 {score+1};
            const int temp_score_11 {score+11};
            if (temp_score_11 > temp_score_1 && temp_score_11 <= 21) {
                score = temp_score_11;
            } else {
                score = temp_score_1;
            }
        } else {
            score += get_rank_score(rank);
        }
    }
};

bool should_continue_game() {
    std::cout << "(h) to hit, or (s) to stand: ";
    char player_answer {};
    std::cin >> player_answer;

    if (player_answer == 'h') {
        return true;
    }
    if (player_answer == 's') {
        return  false;
    }

    std::cout << "wrong answer, try again \n";
    return should_continue_game();
}

void play_round(Player& player, Player& dealer, Deck& deck) {
    bool is_continue_game {should_continue_game()};

    if (is_continue_game == false) {
        while (dealer.score <= 21) {
            auto card = deck.dealCard();

            dealer.add_score(card.rank);

            std::cout << "The dealer flips a " << card << ". They now have: " << dealer.score << '\n';
        }

        if (player.score == dealer.score) {
            std::cout << "neither wins\n";
            return;
        }

        if (player.score == 21) {
            std::cout << "You win!\n";
            return;
        }

        if (dealer.score > 21) {
            std::cout << "The dealer went bust!\n";
            std::cout << "You win!\n";
            return;
        }

        if (player.score > dealer.score) {
            std::cout << "You win!\n";
        } else {
            std::cout << "You lose!\n";
        }

        return;
    }

    Card playerCard {deck.dealCard()};
    player.add_score(playerCard.rank);

    std::cout << "You were dealt " << playerCard << ". You now have: " << player.score << '\n';

    if (player.score > 21) {
        std::cout << "You went bust!\n";
        return;
    }

    play_round(player, dealer, deck);
}

int main() {
    Deck deck{};
    deck.shuffle();

    Player dealer {};
    Player player {};

    Card player_card1 {deck.dealCard()};
    Card player_card2 {deck.dealCard()};
    player.add_score(player_card1.rank);
    player.add_score(player_card2.rank); // додаємо другу карту для гравця

    Card dealerCard {deck.dealCard()};
    dealer.add_score(dealerCard.rank);

    std::cout << "The dealer is showing: " << dealerCard << " (" << dealer.score << ")\n";
    std::cout << "You have score: " << player_card1 << " " << player_card2 << " (" << player.score << ")\n";

    play_round(player, dealer, deck);



    return 0;
}