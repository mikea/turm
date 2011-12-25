#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>

using std::string;

class Board {
};

class Position {
  public:
    bool ParseFEN(const string& fen);
    string ToFEN();
};

#endif __BOARD_H__

