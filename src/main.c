#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_ttf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"


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
  update_board_event board_event;

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
        board_event = move_board(LEFT);
        break;
      case SDLK_RIGHT:
        board_event = move_board(RIGHT);
        break;
      case SDLK_UP:
        board_event = move_board(UP);
        break;
      case SDLK_DOWN:
        board_event = move_board(DOWN);
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

    if (board_event == IDLE)
      continue;

    SDL_SetRenderDrawColor(renderer, 187, 173, 160, 255);
    SDL_RenderClear(renderer);

    spawn_random_block();
    render_board(renderer, &m);

    SDL_RenderPresent(renderer);

    board_event = IDLE;
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
