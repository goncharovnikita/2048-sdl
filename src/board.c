#include "board.h"

board_cell demo_board[ROWS_COUNT * COLS_COUNT] = {
    {.value = C_2},  {.value = C_4},  {.value = C_8},  {.value = C_16},

    {.value = C_4},  {.value = C_8},  {.value = C_16}, {.value = C_32},

    {.value = C_8},  {.value = C_16}, {.value = C_32}, {.value = C_64},

    {.value = C_16}, {.value = C_32}, {.value = C_64}, {.value = C_128},
};

/*
 *  Board represented as two dimensional array:
 *  +-----|-----|-----+
 *  |x0,y0|x1,y0|x2,y0|
 *  |x0,y1|x1,y1|x2,y1|
 *  |x0,y2|x1,y2|x2,y2|
 *  +-----|-----|-----+
 */
board_cell board[ROWS_COUNT * COLS_COUNT] = {
    {},
    {},
    {},
    {},

    {.value = C_8},
    {},
    {.value = C_8},
    {},

    {},
    {},
    {},
    {},

    {},
    {},
    {},
    {},
};

char *get_board_cell_value_str(board_cell_value v) {
  switch (v) {
  case (C_2):
    return "2";
  case (C_4):
    return "4";
  case (C_8):
    return "8";
  case (C_16):
    return "16";
  case (C_32):
    return "32";
  case (C_64):
    return "64";
  case (C_128):
    return "128";
  case (C_256):
    return "256";
  case (C_512):
    return "512";
  case (C_1024):
    return "1024";
  case (C_2048):
    return "2048";
  case (UNKNOWN):
    return "u";
  default:
    return "x";
  }
}

SDL_Color get_board_cell_color(board_cell_value v) {
  SDL_Color color = {.r = 238, .g = 228, .b = 218, .a = 35};
  switch (v) {
  case (C_2):
    color.r = 238;
    color.g = 228;
    color.b = 218;

    break;
  case (C_4):
    color.r = 237;
    color.g = 224;
    color.b = 200;

    break;
  case (C_8):
    color.a = 255;
    color.g = 177;
    color.b = 121;

    break;
  case (C_16):
    color.a = 245;
    color.g = 149;
    color.b = 99;

    break;
  case (C_32):
    color.a = 246;
    color.g = 124;
    color.b = 95;

    break;
  case (C_64):
    color.r = 246;
    color.g = 94;
    color.b = 59;

    break;
  case (C_128):
    color.r = 237;
    color.g = 207;
    color.b = 114;

    break;
  case (C_256):
    color.r = 237;
    color.g = 204;
    color.b = 97;

    break;
  case (C_512):
    color.r = 237;
    color.g = 200;
    color.b = 80;

    break;
  case (C_1024):
    color.r = 237;
    color.g = 197;
    color.b = 63;

    break;
  case (C_2048):
    color.r = 237;
    color.g = 194;
    color.b = 46;

    break;
  default:
    return color;
  }

  return color;
}

board_cell_value get_next_board_cell_value(board_cell_value v) {
  switch (v) {
  case (C_2):
    return C_4;
  case (C_4):
    return C_8;
  case (C_8):
    return C_16;
  case (C_16):
    return C_32;
  case (C_32):
    return C_64;
  case (C_64):
    return C_128;
  case (C_128):
    return C_256;
  case (C_256):
    return C_512;
  case (C_512):
    return C_1024;
  case (C_1024):
    return C_2048;
  default:
    return UNKNOWN;
  }
}

uint8_t is_board_cell_empty(board_cell cell) { return cell.value == UNKNOWN; }

/*
 *  (0,0,0),  (1,0,1),  (2,0,2),  (3,0,3)
 *  (0,1,4),  (1,1,5),  (2,1,6),  (3,1,7)
 *  (0,2,8),  (1,2,9),  (2,2,10), (3,2,11)
 *  (0,3,12), (1,3,13), (2,3,14), (3,3,15)
 */
board_cell *get_board_cell(uint8_t x, uint8_t y) {
  uint8_t index = (ROWS_COUNT * y) + x;

  if (index > ROWS_COUNT * COLS_COUNT) {
    return NULL;
  }

  return &board[index];
}

void update_board_cell(uint8_t x, uint8_t y, board_cell *cell) {
  uint8_t index = (ROWS_COUNT * y) + x;

  if (index > ROWS_COUNT * COLS_COUNT) {
    return;
  }

  if (NULL == cell) {
    board[index].value = UNKNOWN;

    return;
  }

  board[index].value = cell->value;
}

void debug_print_board() {
  printf("+");

  for (int i = 0; i < ROWS_COUNT; i++) {
    printf("--");
  }

  printf("+\n");

  for (int y = 0; y < ROWS_COUNT; y++) {
    printf("|");

    for (int x = 0; x < COLS_COUNT; x++) {
      board_cell *cell = get_board_cell(x, y);

      if (NULL == cell)
        continue;

      printf("%s|", get_board_cell_value_str(cell->value));
    }

    printf("\n");
  }

  printf("+");

  for (int i = 0; i < ROWS_COUNT; i++) {
    printf("--");
  }

  printf("+\n");

  printf("\n");
}

/*
 * Movement:
 * Iterating counter-directioin wise from @direction
 * * Found empty cell:
 * * Use another iterator to find non-empty cell
 * * Move next non-empty cell to current pos, increment iterator by 1
 * * If non-empty cell not found, finish row movement algorithm
 */
update_board_event move_board(move_direction direction) {
  update_board_event event = IDLE;

  board_cell *slow_cell;
  board_cell *fast_cell;
  uint8_t slow_empty;
  uint8_t fast_empty;
  uint8_t horizontal_movement = direction == LEFT || direction == RIGHT;

  int8_t x = 0;
  int8_t y = 0;
  int8_t x1 = 0;
  int8_t y1 = 0;

  int8_t *fast_x = &x1;
  int8_t *fast_y = &y;
  int8_t *direction_fast_index = &x1;
  int8_t *direction_slow_index = &x;
  int8_t *perp_index = &y;
  int8_t fast_index_increment = 1;
  int8_t perp_index_increment = 1;

  if (direction == RIGHT) {
    x = COLS_COUNT - 1;
    x1 = COLS_COUNT - 1;
    fast_index_increment = -1;
    perp_index_increment = -1;
  }

  if (direction == DOWN) {
    y = ROWS_COUNT - 1;
    y1 = ROWS_COUNT - 1;
    fast_index_increment = -1;
    perp_index_increment = -1;
  }

  if (!horizontal_movement) {
    fast_x = &x;
    fast_y = &y1;
    direction_fast_index = &y1;
    direction_slow_index = &y;
    perp_index = &x;
  }

  for (;;) {
    SDL_Log("x: %d, y: %d, x1: %d, y1: %d", x, y, x1, y1);

    if (y >= ROWS_COUNT && horizontal_movement) {
      break;
    } else if (x >= COLS_COUNT && !horizontal_movement) {
      break;
    }

    slow_cell = get_board_cell(x, y);
    fast_cell = get_board_cell(*fast_x, *fast_y);

    if (slow_cell == NULL || fast_cell == NULL)
      break;

    slow_empty = is_board_cell_empty(*slow_cell);
    fast_empty = is_board_cell_empty(*fast_cell);

    if (*direction_fast_index != *direction_slow_index) {
      // Move cell to empty cell
      if (slow_empty && !fast_empty) {
        update_board_cell(x, y, fast_cell);
        update_board_cell(*fast_x, *fast_y, NULL);

        event = UPDATED;

        if (direction == LEFT) {
          if (x > 0) {
            x1 = x;
            x--;
          } else {
            x1++;
          }
        } else if (direction == RIGHT) {
          if (x != COLS_COUNT - 1) {
            x1 = x;
            x++;
          } else {
            x1--;
          }
        } else if (direction == UP) {
          if (y > 0) {
            y1 = y;
            y--;
          } else {
            y1++;
          }
        } else if (direction == DOWN) {
          if (y != ROWS_COUNT - 1) {
            y1 = y;
            y++;
          } else {
            y1--;
          }
        }
        // Merge cells
      } else if (!slow_empty && !fast_empty &&
                 slow_cell->value == fast_cell->value) {
        slow_cell->value = get_next_board_cell_value(slow_cell->value);
        update_board_cell(*fast_x, *fast_y, NULL);

        if (direction == LEFT) {
          x++;
          x1++;
        } else if (direction == RIGHT) {
          x--;
          x1--;
        } else if (direction == UP) {
          y++;
          y1++;
        } else if (direction == DOWN) {
          y--;
          y1--;
        }

        event = UPDATED;
        // Move slow index to find empty cell
      } else if (!slow_empty) {
        if (direction == LEFT) {
          x++;
        } else if (direction == RIGHT) {
          x--;
        } else if (direction == UP) {
          y++;
        } else if (direction == DOWN) {
          y--;
        }
        // Move fast index
      } else {
        if (direction == LEFT) {
          x1++;
        } else if (direction == RIGHT) {
          x1--;
        } else if (direction == UP) {
          y1++;
        } else if (direction == DOWN) {
          y1--;
        }
      }
    } else {
      if (direction == LEFT) {
        x1++;
      } else if (direction == RIGHT) {
        x1--;
      } else if (direction == UP) {
        y1++;
      } else if (direction == DOWN) {
        y1--;
      }
    }

    if (x1 == COLS_COUNT && direction == LEFT) {
      x1 = 0;
      x = 0;
      y++;
    }

    if (x1 < 0 && direction == RIGHT) {
      x1 = COLS_COUNT - 1;
      x = COLS_COUNT - 1;
      y++;
    }

    if (y1 >= ROWS_COUNT && direction == UP) {
      y1 = 0;
      y = 0;
      x++;
    }

    if (y1 < 0 && direction == DOWN) {
      y1 = ROWS_COUNT - 1;
      y = ROWS_COUNT - 1;
      x++;
    }
  }

  return event;
}
