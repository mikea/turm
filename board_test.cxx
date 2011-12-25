#include "gtest/gtest.h"
#include "board.h"

using std::string;

// Test fen parsing/generation.
class FENTest : public ::testing::Test {
  public:
    string RunTest(const string& test) {
      Position position;
      if (!position.ParseFEN(test)) {
        return "ERROR";
      }
      return position.ToFEN();
    }
};

TEST_F(FENTest, TestInitialPosition) {
    EXPECT_EQ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        RunTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
}

