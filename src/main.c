#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_ttf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 80.0
#define GAP 40.0
#define BLOCK_TEXT_SIZE 40.0
#define ROWS_COUNT 4
#define COLS_COUNT 4
#define WINDOW_HEIGHT BLOCK_SIZE *ROWS_COUNT + GAP * 2.0 + GAP *(ROWS_COUNT - 1)
#define WINDOW_WIDTH BLOCK_SIZE *COLS_COUNT + GAP * 2.0 + GAP *(COLS_COUNT - 1)
#define BLOCK_SPAWN_ANIMATION_DURATION_MILLIS 200

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
uint8_t move_board(move_direction direction) {
  uint8_t board_updated = 0;

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

  int8_t direction_index_zero_value = 0;
  int8_t direction_index_max_value = COLS_COUNT;

  if (direction == RIGHT) {
    x = COLS_COUNT - 1;
    x1 = COLS_COUNT - 1;
    fast_index_increment = -1;
    direction_index_zero_value = COLS_COUNT - 1;
    direction_index_max_value = -1;
  }

  if (direction == DOWN) {
    y = ROWS_COUNT - 1;
    y1 = ROWS_COUNT - 1;
    fast_index_increment = -1;
    direction_index_zero_value = ROWS_COUNT - 1;
    direction_index_max_value = -1;
  }

  if (!horizontal_movement) {
    fast_x = &x;
    fast_y = &y1;
    direction_fast_index = &y1;
    direction_slow_index = &y;
    perp_index = &x;
  }

  for (;;) {
    int8_t direction_index_start_position =
        (*direction_slow_index + -fast_index_increment < 0) ||
        (horizontal_movement &&
             (*direction_slow_index + -fast_index_increment == COLS_COUNT) ||
         !horizontal_movement &&
             (*direction_slow_index + -fast_index_increment == ROWS_COUNT));

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

        if (direction_index_start_position) {
          *direction_fast_index += fast_index_increment;
        } else {
          // If two blocks can merge after movement, they will
          *direction_fast_index = *direction_slow_index;
          *direction_slow_index += -fast_index_increment;
        }

        board_updated = 1;
        // Merge cells
      } else if (!slow_empty && !fast_empty &&
                 slow_cell->value == fast_cell->value) {
        slow_cell->value = get_next_board_cell_value(slow_cell->value);
        update_board_cell(*fast_x, *fast_y, NULL);

        *direction_slow_index += fast_index_increment;
        *direction_fast_index += fast_index_increment;

        board_updated = 1;
        // Move slow index to find empty cell
      } else if (!slow_empty) {
        *direction_slow_index += fast_index_increment;
        // Move fast index
      } else {
        *direction_fast_index += fast_index_increment;
      }
    } else {
      *direction_fast_index += fast_index_increment;
    }

    if (*direction_fast_index == direction_index_max_value) {
      *direction_fast_index = direction_index_zero_value;
      *direction_slow_index = direction_index_zero_value;
      *perp_index += 1;
    }
  }

  return board_updated;
}


void spawn_random_block() {
  board_cell *empty_cells[ROWS_COUNT * COLS_COUNT];
  uint8_t empty_cells_size = 0;

  for (int y = 0; y < ROWS_COUNT; y++) {
    for (int x = 0; x < COLS_COUNT; x++) {
      board_cell *cell = get_board_cell(x, y);

      if (cell == NULL)
        continue;

      if (is_board_cell_empty(*cell)) {
        empty_cells[empty_cells_size] = cell;
        empty_cells_size++;
      }
    }
  }

  if (empty_cells_size == 0)
    return;

  int cell_index = rand() % empty_cells_size;

  if (rand() >= RAND_MAX / 2) {
    empty_cells[cell_index]->value = C_4;

    return;
  }

  empty_cells[cell_index]->value = C_2;
}

void render_text(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect rect,
                 char *text) {
  SDL_Color White = {255, 255, 255};

  SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, White);
  SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

  SDL_RenderCopy(renderer, Message, NULL, &rect);

  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
}

void render_board(SDL_Renderer *renderer, materials *m) {
  for (int y = 0; y < ROWS_COUNT; y++) {
    for (int x = 0; x < COLS_COUNT; x++) {
      board_cell *cell = get_board_cell(x, y);

      if (NULL == cell)
        continue;

      SDL_Rect rect;
      rect.x = (BLOCK_SIZE * x) + (GAP * (x + 1));
      rect.y = (BLOCK_SIZE * y) + (GAP * (y + 1));
      rect.w = BLOCK_SIZE;
      rect.h = BLOCK_SIZE;

      SDL_Color color = get_board_cell_color(cell->value);

      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
      SDL_RenderFillRect(renderer, &rect);

      if (cell->value != UNKNOWN) {
        render_text(renderer, m->primary_text_font, rect,
                    get_board_cell_value_str(cell->value));
      }
    }
  }
}

int start_sdl_loop() {
  char quit = 0;
  SDL_Event event;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window =
      SDL_CreateWindow("2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  int ttf_inited = TTF_Init();

  if (ttf_inited != 0) {
    printf("ttf init failed\n");

    return ttf_inited;
  }

  TTF_Font *font = TTF_OpenFont("assets/Roboto-Bold.ttf", 24);

  materials m;

  m.primary_text_font = font;

  SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
  SDL_RenderClear(renderer);

  render_board(renderer, &m);

  SDL_RenderPresent(renderer);

  int pressed_key = 0;
  uint8_t board_updated;

  while (quit != 1) {
    SDL_Delay(10);
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      quit = 1;
      break;
    case SDL_KEYDOWN:
      if (pressed_key != 0)
        continue;

      pressed_key = event.key.keysym.sym;

      switch (pressed_key) {
      case SDLK_LEFT:
        board_updated = move_board(LEFT);
        break;
      case SDLK_RIGHT:
        board_updated = move_board(RIGHT);
        break;
      case SDLK_UP:
        board_updated = move_board(UP);
        break;
      case SDLK_DOWN:
        board_updated = move_board(DOWN);
        break;
      default:
        break;
      }

      break;
    case SDL_KEYUP:
      if (pressed_key == 0)
        continue;

      if (event.key.keysym.sym == pressed_key) {
        pressed_key = 0;
      }

      break;
    default:
      break;
    }

    if (!board_updated)
      continue;

    SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
    SDL_RenderClear(renderer);

    spawn_random_block();
    render_board(renderer, &m);

    SDL_RenderPresent(renderer);

    board_updated = 0;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

int main(int argc, const char *argv[]) {
  debug_print_board();

  return start_sdl_loop();
}
