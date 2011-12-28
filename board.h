#ifndef __BOARD_H__
#define __BOARD_H__

#include <boost/utility.hpp>
#include <stdint.h>
#include <string>
#include <utility>

using std::string;


enum Piece {
  NONE,
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING
};

enum Color { BLACK, WHITE };

class Board : boost::noncopyable {
  public:
    Board();

    void Set(int x, int y, Color color, Piece piece);

    std::pair<Color, Piece> Get(int x, int y);

  private:
    int8_t array_[64];
};

class Position : boost::noncopyable {
  public:
    Position();

    bool ParseFEN(const string& fen, string* error);
    string ToFEN();
  private:
    Board board_;
    Color active_color_;

    bool w_castle_k_;
    bool w_castle_q_;
    bool b_castle_k_;
    bool b_castle_q_;

    int halfmoves_;
    int fullmoves_;
};

#endif __BOARD_H__

