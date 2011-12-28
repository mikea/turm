#include <ctype.h>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <iostream>
#include <string>

#include <iostream>

#include "board.h"

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

using qi::ushort_;
using qi::_1;
using ascii::space;
using phoenix::ref;


Board::Board() {
    memset(array_, 0, sizeof(array_));
}

void Board::Set(int x, int y, Color color, Piece piece) {
  BOOST_ASSERT(x >= 0 && x <= 7);
  BOOST_ASSERT(y >= 0 && y <= 7);
  int8_t p = static_cast<int8_t>(piece);

  if (color == BLACK) {
    p = -p;
  }

  array_[y * 8 + x] = p;
}

std::pair<Color, Piece> Board::Get(int x, int y) {
  int8_t v = array_[y * 8 + x];
  Color c = WHITE;

  if (v < 0) {
    c = BLACK;
    v = -v;
  }

  return std::make_pair(c, static_cast<Piece>(v));
}

Position::Position()
  : active_color_(WHITE),
    w_castle_k_(false),
    w_castle_q_(false),
    b_castle_k_(false),
    b_castle_q_(false),
    halfmoves_(0),
    fullmoves_(1) {}

bool Position::ParseFEN(const string& fen,
                        string* error) {
  error->clear();

  // TODO: figure out a way to use qi for this too.
  int pos = 0;
  for (int x = 0, y = 7; x < 8 || y > 0; ++pos, ++x) {
    char ch = fen[pos];
    Color c = isupper(ch) ? WHITE : BLACK;

    switch (ch) {
      default:
        *error = string("ERROR: bad char:") + fen[pos] +
          boost::lexical_cast<string>(x) + "-" +
          boost::lexical_cast<string>(y);
        return false;
      case 'p':
      case 'P':
        board_.Set(x, y, c, PAWN);
        break;
      case 'n':
      case 'N':
        board_.Set(x, y, c, KNIGHT);
        break;
      case 'b':
      case 'B':
        board_.Set(x, y, c, BISHOP);
        break;
      case 'r':
      case 'R':
        board_.Set(x, y, c, ROOK);
        break;
      case 'q':
      case 'Q':
        board_.Set(x, y, c, QUEEN);
        break;
      case 'k':
      case 'K':
        board_.Set(x, y, c, KING);
        break;
      case '/':
        if (y == 0) {
          *error = string("ERROR: y is 0");
          return false;
        }
        if (x != 8) {
          *error = string("ERROR: x is ") +
            boost::lexical_cast<string>(x) +
            " at " + boost::lexical_cast<string>(pos);
          return false;
        }
        --y;
        x = -1;
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
        x += ch - '1';
        break;
    }
  }

  halfmoves_ = 0;
  fullmoves_ = 1;

  std::string::const_iterator start = fen.begin() + pos;
  std::string::const_iterator end = fen.end();
  bool r = qi::phrase_parse(start, end,
       // Begin grammar
       (
        " " >>
        // Active color
        (
         (qi::string("b")[ref(active_color_) = BLACK]) |
         (qi::string("w")[ref(active_color_) = WHITE])
        ) >>
        " " >>
        // Castling
        (
         (qi::string("-")[
            ref(w_castle_k_) = false,
            ref(w_castle_q_) = false,
            ref(b_castle_k_) = false,
            ref(b_castle_q_) = false]) |
         (
          -(qi::string("K")[ref(w_castle_k_) = true]) >>
          -(qi::string("Q")[ref(w_castle_q_) = true]) >>
          -(qi::string("k")[ref(b_castle_k_) = true]) >>
          -(qi::string("q")[ref(b_castle_q_) = true])
         )
        ) >>
        // En passant
        " -" >>
        -(
         " " >>
         // halfmoves
         ushort_[ref(halfmoves_) = _1] >>
         " " >>
         // fullmoves
         ushort_[ref(fullmoves_) = _1]
        )
       ),
       qi::eoi);

  if (!r) {
    *error = "parse error";
    return false;
  }

  if (start != end) {
    *error = "parse error: extra characters remained";
    return false;
  }

  return true;
}

string Position::ToFEN() {
  string result;

  int empty;

  for (int y = 7; y >=0; --y) {
    empty = 0;
    for (int x = 0; x < 8; ++x) {
      std::pair<Color, Piece> cp = board_.Get(x, y);

      if (empty > 0 && cp.second != NONE) {
        result += boost::lexical_cast<string>(empty);
        empty = 0;
      }

      char ch = 0;

      switch (cp.second) {
        case PAWN:
          ch = 'p';
          break;
        case KNIGHT:
          ch = 'n';
          break;
        case BISHOP:
          ch = 'b';
          break;
        case ROOK:
          ch = 'r';
          break;
        case QUEEN:
          ch = 'q';
          break;
        case KING:
          ch = 'k';
          break;
        case NONE:
          ++empty;
          break;
      }

      if (ch != 0) {
        if (cp.first == WHITE) {
          ch = toupper(ch);
        }
        result += ch;
      }
    }
    if (empty > 0) {
      result += boost::lexical_cast<string>(empty);
    }
    if (y != 0) {
      result += '/';
    }
  }

  // TODO: use spirit karma.

  // Active color
  result += ' ';
  if (active_color_ == WHITE) {
    result += 'w';
  } else {
    result += 'b';
  }

  // Castling
  result += ' ';
  if (!b_castle_q_ && !b_castle_k_ && !w_castle_k_ && !w_castle_q_) {
    result += '-';
  } else {
    if (w_castle_k_) {
      result += 'K';
    }
    if (w_castle_q_) {
      result += 'Q';
    }
    if (b_castle_k_) {
      result += 'k';
    }
    if (b_castle_q_) {
      result += 'q';
    }
  }

  // En passant
  result += ' ';
  result += '-';

  result += ' ';
  result += boost::lexical_cast<string>(halfmoves_);
  result += ' ';
  result += boost::lexical_cast<string>(fullmoves_);


  return result;
}


