#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <vector>


/**
 * Задача
 * Реалізувати гру 15 Puzzle!
 * Щоб загадати правила можна пограти у неї тут https://15puzzle.netlify.app/
 */

using Matrix = std::vector<std::vector<int>>;
constexpr int BOARD_ROW {4};
constexpr int BOARD_COL {4};

enum Direction {up, down, left, right};

namespace Random {
 // Returns a seeded Mersenne Twister
 // Note: we'd prefer to return a std::seed_seq (to initialize a std::mt19937), but std::seed can't be copied, so it can't be returned by value.
 // Instead, we'll create a std::mt19937, seed it, and then return the std::mt19937 (which can be copied).
 inline std::mt19937 generate() {
   std::random_device rd{};

   // Create seed_seq with clock and 7 random numbers from std::random_device
   std::seed_seq ss{
     static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
       rd(), rd(), rd(), rd(), rd(), rd(), rd()
   };

   return std::mt19937{ ss };
 }
}

struct Point {
  int x{};
  int y{};
};


struct GameStatus {
  bool is_game_active {true};
  bool is_user_win {false};
  int move_count {0};
};

class Board {
private:
  Point empty_point {0,0};
  Matrix board;
  GameStatus game_status {};


  void generate_board() {
    constexpr int flat_board_size {BOARD_COL * BOARD_ROW};
    std::vector<int> flat(flat_board_size);
    for (int i = 0; i < flat_board_size; ++i) flat[i] = i;

    /**
     * Gemini говорить, що shuffle тут не підійде бо у цій грі є комбінації, які неможливо виконати,
     * тому кращими варіантом буде починати з початкового відсортованого варіанту і потім випадковим чином запустити
     * 1000 ходів щоб отримати рандомний розподіл. Але для прикладу я лишу цю реалізацію, але раптом що,
     * це не складно, треба лише
     *  1) закоментити shuffle
     *  2) board генеруєть з початковим значенням відсортованого масиву масивів
     *  3) зробити можливість генерувати випадковий хід
     *  4) запустити випадкові ходи 1000 раз
     */
    std::mt19937 mt {Random::generate()};
    std::ranges::shuffle(flat, mt);

    // Ресайзимо матрицю одним махом
    board.assign(BOARD_ROW, std::vector<int>(BOARD_COL));

    int index = 0;
    for (int row = 0; row < BOARD_ROW; ++row) {
      for (int col = 0; col < BOARD_COL; ++col) {
        board[row][col] = flat[index];
        if (board[row][col] == 0) {
          empty_point={col, row};
        }
        index++;
      }
    }
  }


  void swap_empty_point(const Point target) {

    std::swap(board[empty_point.y][empty_point.x], board[target.y][target.x]);

    empty_point = target;
    game_status.move_count++;
  }

public:
  Board() {
    generate_board();
  }

  void ask_user_about_move() {
    std::string user_input {};
    std::cout << "enter your move: ";
    std::cin >> user_input;

    Direction user_move {};

    if (user_input == "q") {
      game_status.is_game_active = false;
      std::cout << "goodbye\n";
      std::cout << "you made " << game_status.move_count << " moves\n";
      return;
    }

    if (user_input == "up" || user_input == "w") {
      user_move = Direction::up;
    } else if (user_input == "down" || user_input == "s") {
      user_move = Direction::down;
    } else if (user_input == "left" || user_input == "a") {
      user_move = Direction::left;
    } else if (user_input == "right" || user_input == "d") {
      user_move = Direction::right;
    } else {
      std::cout << "unknown command\n";
    }

    move(user_move);
  }

  void move(const Direction move) {
    auto [x, y] {empty_point};

    switch (move) {
    case up:
      if (y > 0) {
        swap_empty_point({x, y-1});
      } else {
        std::cout << "impossible move\n";
      }
      break;
    case down:
      if (y < BOARD_ROW-1) {
        swap_empty_point({x, y+1});
      } else {
        std::cout << "impossible move\n";
      }
      break;
    case left:
      if (x > 0) {
        swap_empty_point({x-1, y});
      } else {
        std::cout << "impossible move\n";
      }
      break;
    case right:
      if (x < BOARD_COL-1) {
        swap_empty_point({x+1, y});
      } else {
        std::cout << "impossible move\n";
      }
      break;
    }

    check_game_status();
  }

  void check_game_status() {
    if (is_board_sorted()) {
      game_status.is_game_active = false;
      game_status.is_user_win = true;
    }
  }

  [[nodiscard]] bool is_board_sorted() const {
    int counter = 1;
    for (int y = 0; y < BOARD_ROW; ++y) {
      for (int x = 0; x < BOARD_COL; ++x) {
        if (y == BOARD_ROW - 1 && x == BOARD_COL - 1) return board[y][x] == 0;
        if (board[y][x] != counter++) return false;
      }
    }

    return true;
  }

  void print() const {
    for (int row {0}; row < BOARD_ROW; ++row) {
      for (int col {0}; col < BOARD_COL; ++col) {
        const int value {board[row][col]};
        std::string space_before = value > 9 ? " " : "  ";
        std::cout << space_before << value << "   ";
      }
      std::cout << '\n';
    }
  }

  [[nodiscard]] GameStatus get_game_status() const {
    return game_status;
  }


};



int main() {
  Board board {};

  while (board.get_game_status().is_game_active) {
    board.print();
    board.ask_user_about_move();
  }


  return 0;
}