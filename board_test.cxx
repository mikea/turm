#include "gtest/gtest.h"
#include "board.h"

using std::string;

// Test fen parsing/generation.
class FENTest : public ::testing::Test {
  public:
    string RunTest(const string& test) {
      Position position;
      string error;

      if (!position.ParseFEN(test, &error)) {
        return error;
      }
      return position.ToFEN();
    }
};

TEST_F(FENTest, TestFen) {
  EXPECT_EQ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
      RunTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));

  EXPECT_EQ("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
      RunTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -"));

  EXPECT_EQ("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 4 5",
      RunTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 4 5"));

  EXPECT_EQ("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
      RunTest("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"));

  EXPECT_EQ("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1",
    RunTest("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1"));
}

