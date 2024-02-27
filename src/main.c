#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_ttf.h>
#include <stdint.h>
#include <stdio.h>

#define BLOCK_SIZE 80.0
#define GAP 40.0
#define BLOCK_TEXT_SIZE 40.0
#define ROWS_COUNT 4
#define COLS_COUNT 4
#define WINDOW_HEIGHT BLOCK_SIZE *ROWS_COUNT + GAP * 2.0 + GAP *(ROWS_COUNT - 1)
#define WINDOW_WIDTH BLOCK_SIZE *COLS_COUNT + GAP * 2.0 + GAP *(COLS_COUNT - 1)
#define BLOCK_SPAWN_ANIMATION_DURATION_MILLIS 200

enum board_cell_value {
  UNKNOWN,
  C_2,
  C_4,
  C_8,
  C_16,
  C_32,
  C_64,
  C_128,
};

typedef enum board_cell_value board_cell_value;

enum update_board_event {
  NOT_UPDATED,
  UPDATED,
};

typedef enum update_board_event update_board_event;

enum move_direction {
  UP,
  RIGHT,
  DOWN,
  LEFT,
};

typedef enum move_direction move_direction;

struct board_cell {
  enum board_cell_value value;
};

struct materials {
  TTF_Font *primary_text_font;
};

typedef struct materials materials;

/*
 *  Board represented as two dimensional array:
 *  +-----|-----|-----+
 *  |x0,y0|x1,y0|x2,y0|
 *  |x0,y1|x1,y1|x2,y1|
 *  |x0,y2|x1,y2|x2,y2|
 *  +-----|-----|-----+
 */
struct board_cell board[ROWS_COUNT * COLS_COUNT] = {
    {.value = C_2},  {.value = C_4},  {.value = C_8},  {.value = C_16},

    {.value = C_4},  {.value = C_8},  {.value = C_16}, {.value = C_32},

    {.value = C_8},  {.value = C_16}, {.value = C_32}, {.value = C_64},

    {.value = C_16}, {.value = C_32}, {.value = C_64}, {.value = C_128},
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
  default:
    return "x";
  }
}

SDL_Color get_board_cell_color(enum board_cell_value v) {
  SDL_Color color;
  /* _256_color: (237, 204, 97)), */
  /* _512_color: (237, 200, 80)), */
  /* _1024_color: (237, 197, 63)), */
  /* _2048_color: (237, 194, 46)), */
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
  default:
    return color;
  }

  return color;
}

/*
 *  (0,0,0),  (1,0,1),  (2,0,2),  (3,0,3)
 *  (0,1,4),  (1,1,5),  (2,1,6),  (3,1,7)
 *  (0,2,8),  (1,2,9),  (2,2,10), (3,2,11)
 *  (0,3,12), (1,3,13), (2,3,14), (3,3,15)
 */
struct board_cell *get_board_cell(uint8_t x, uint8_t y) {
  uint8_t index = (ROWS_COUNT * y) + x;

  if (index > ROWS_COUNT * COLS_COUNT) {
    return NULL;
  }

  return &board[index];
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
      struct board_cell *cell = get_board_cell(x, y);

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

void render_text(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect rect,
                 char *text) {
  SDL_Color White = {255, 255, 255};

  SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, White);
  SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

  SDL_RenderCopy(renderer, Message, NULL, &rect);

  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
}

void draw_board(SDL_Renderer *renderer, materials *m) {
  for (int y = 0; y < ROWS_COUNT; y++) {
    for (int x = 0; x < COLS_COUNT; x++) {
      struct board_cell *cell = get_board_cell(x, y);

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

      render_text(renderer, m->primary_text_font, rect,
                  get_board_cell_value_str(cell->value));
    }
  }
}

update_board_event move_board(move_direction direction) {

  return NOT_UPDATED;
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

  draw_board(renderer, &m);

  SDL_RenderPresent(renderer);

  int pressed_key = 0;

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
        SDL_Log("left");
        break;
      case SDLK_RIGHT:
        SDL_Log("right");
        break;
      case SDLK_UP:
        SDL_Log("up");
        break;
      case SDLK_DOWN:
        SDL_Log("down");
        break;
      default:
        break;
      }

      break;
    case SDL_KEYUP:
        SDL_Log("keyup");
      if (pressed_key == 0)
        continue;

      if (event.key.keysym.sym == pressed_key) {
        pressed_key = 0;
      }

      break;
    default:
      break;
    }

    /* SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255); */
    /* SDL_RenderClear(renderer); */

    /* draw_board(renderer, &m); */

    // SDL_RenderPresent(renderer);
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
