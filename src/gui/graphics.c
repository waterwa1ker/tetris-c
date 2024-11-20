/**
 * @file graphics.c
 * @brief Main frontend functions of the game
 * @author nataliak
 */

#include "../tetris/tetris.h"

/**
 * @ingroup graphics_funcs
 * @brief Rendering of game elements, such as: field, current piece, next piece,
 * statistics
 */
void print_game() {
  GameInfo_t *stats = updateCurrentState();
  clear_info();
  mvprintw(1, 25, "NEXT:");
  mvprintw(8, 25, "SCORE:");
  mvprintw(10, 25, "BEST:");
  mvprintw(12, 25, "LEVEL:");
  mvprintw(14, 25, "SPEED:");

  clear_field();
  print_tetromino();
  print_field();

  mvprintw(8, 32, "%d", stats->score);
  mvprintw(10, 31, "%d", stats->high_score);
  mvprintw(12, 32, "%d", stats->level);
  mvprintw(14, 32, "%d", stats->speed);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mvprintw(j + 3, i * 2 + 26, "  ");
      if (stats->next_tetromino.tet[i][j] == 1) {
        mvprintw(j + 3, i * 2 + 26, "[]");
      }
    }
  }
  for (int i = 1; i <= 20; i++) {
    mvaddch(21, i, ACS_HLINE);
    mvaddch(0, i, ACS_HLINE);
  }
}

/**
 * @ingroup graphics_funcs
 * @brief Info block clearing
 */
void clear_info() {
  for (int i = 11; i < 25; i++) {
    for (int j = 1; j < 21; j++) {
      mvprintw(j, i * 2 + 1, "  ");
    }
  }
}

/**
 * @ingroup graphics_funcs
 * @brief Field block clearing
 */
void clear_field() {
  for (int i = 0; i < 10; i++) {
    for (int j = 1; j < 21; j++) {
      mvprintw(j, i * 2 + 1, "  ");
    }
  }
}

/**
 * @ingroup graphics_funcs
 * @brief Field rendering
 */
void print_field() {
  GameInfo_t *stats = updateCurrentState();
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      if (stats->field[i][j] == 1) {
        mvprintw(j + 1, i * 2 + 1, "[]");
      }
    }
  }
}

/**
 * @ingroup graphics_funcs
 * @brief Rendering of the current tetromino
 */
void print_tetromino() {
  GameInfo_t *stats = updateCurrentState();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (stats->current_tetromino.tet[i][j] == 1) {
        mvprintw((stats->cur_y + j), (stats->cur_x + i) * 2 + 1, "[]");
      }
    }
  }
}

/**
 * @ingroup graphics_funcs
 * @brief Rendering a game over banner
 */
void print_banner() {
  GameInfo_t *stats = updateCurrentState();
  clear_field();
  mvprintw(6, 6, "GAME OVER");
  mvprintw(8, 6, "SCORE: %d", stats->score);
  mvprintw(10, 5, "PRESS ENTER");
  mvprintw(11, 5, "TO TRY AGAIN");
  mvprintw(13, 6, "PRESS 'Q'");
  mvprintw(14, 7, "TO QUIT");
}

/**
 * @ingroup graphics_funcs
 * @brief Rendering start banner
 */
void print_start_banner() {
  mvprintw(10, 5, "PRESS ENTER");
  mvprintw(6, 25, "CONTROLS");
  mvprintw(8, 25, "ENTER - START");
  mvprintw(9, 25, "SPACE - ACTION");
  mvprintw(10, 25, "ARROW DOWN - SHIFT DOWN");
  mvprintw(11, 25, "ARROW LEFT - SHIFT LEFT");
  mvprintw(12, 25, "ARROW RIGHT - SHIFT RIGHT");
  mvprintw(13, 25, "'P' - PAUSE");
  mvprintw(14, 25, "'Q' - QUIT");
}

/**
 * @ingroup graphics_funcs
 * @brief Rendering of graphics based on state
 * @param[in] state Current game state
 */
void print_something(FSM_STATES_g state) {
  if (state == GAME_OVER) {
    print_banner();
  } else if (state == START) {
    print_start_banner();
  } else {
    print_game();
  }
}