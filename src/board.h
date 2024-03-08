#include <SDL_ttf.h>

#define BLOCK_SIZE 80.0
#define GAP 40.0
#define BLOCK_TEXT_SIZE 40.0
#define ROWS_COUNT 4
#define COLS_COUNT 4
#define WINDOW_HEIGHT BLOCK_SIZE *ROWS_COUNT + GAP * 2.0 + GAP *(ROWS_COUNT - 1)
#define WINDOW_WIDTH BLOCK_SIZE *COLS_COUNT + GAP * 2.0 + GAP *(COLS_COUNT - 1)
#define BLOCK_SPAWN_ANIMATION_DURATION_MILLIS 200

typedef enum {
  IDLE,
  UPDATED,
} update_board_event;

typedef enum {
  UP,
  RIGHT,
  DOWN,
  LEFT,
} move_direction;

typedef enum {
  UNKNOWN,
  C_2,
  C_4,
  C_8,
  C_16,
  C_32,
  C_64,
  C_128,
  C_256,
  C_512,
  C_1024,
  C_2048,
} board_cell_value;

typedef struct {
  board_cell_value value;
} board_cell;

typedef struct {
  TTF_Font *primary_text_font;
} materials;

board_cell board[ROWS_COUNT * COLS_COUNT];

update_board_event move_board(move_direction direction);
char *get_board_cell_value_str(board_cell_value v);
SDL_Color get_board_cell_color(board_cell_value v);
board_cell_value get_next_board_cell_value(board_cell_value v);
uint8_t is_board_cell_empty(board_cell cell);
board_cell *get_board_cell(uint8_t x, uint8_t y);
void update_board_cell(uint8_t x, uint8_t y, board_cell *cell);
void debug_print_board();
