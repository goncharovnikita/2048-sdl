#include <stdlib.h>
#include <stdio.h>

#include "../include/Unity/src/unity.h"
#include "../src/board.h"

board_cell expected_board[ROWS_COUNT * COLS_COUNT];

void setUp(void)
{
  for (int y = 0; y < ROWS_COUNT; y++) {
    for (int x = 0; y < COLS_COUNT; x++) {
      update_board_cell(x, y, NULL);
    }
  }
}

void tearDown(void)
{
}

void test_move_board(void)
{
  for (int y = 0; y < ROWS_COUNT; y++) {
    for (int x = 0; y < COLS_COUNT; x++) {
      board_cell *actual_cell = get_board_cell(x, y);
    }
  }

  TEST_ASSERT_EQUAL(0, 0);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_move_board);
    return UNITY_END();
}
