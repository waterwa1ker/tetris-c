/**
 * @file tetris.c
 * @brief Backend игры
 * @author nataliak
 */

#include "tetris.h"

/**
 * @brief Фигуры тетриса
 * @param[in] num Индекс фигуры
 * @return Структура фигуры
 */
tetromino get_tetromino(int num) {
  tetromino tet = {0};
  tet.type = num;
  switch (num) {
    case 0:
      tet.tet[1][0] = 1;
      tet.tet[1][1] = 1;
      tet.tet[1][2] = 1;
      tet.tet[1][3] = 1;
      break;
    case 1:
      tet.tet[0][0] = 1;
      tet.tet[0][1] = 1;
      tet.tet[1][0] = 1;
      tet.tet[1][1] = 1;
      break;
    case 2:
      tet.tet[0][0] = 1;
      tet.tet[0][1] = 1;
      tet.tet[1][1] = 1;
      tet.tet[2][1] = 1;
      break;
    case 3:
      tet.tet[2][0] = 1;
      tet.tet[0][1] = 1;
      tet.tet[1][1] = 1;
      tet.tet[2][1] = 1;
      break;
    case 4:
      tet.tet[0][0] = 1;
      tet.tet[1][0] = 1;
      tet.tet[1][1] = 1;
      tet.tet[2][1] = 1;
      break;
    case 5:
      tet.tet[0][1] = 1;
      tet.tet[1][0] = 1;
      tet.tet[1][1] = 1;
      tet.tet[2][0] = 1;
      break;
    case 6:
      tet.tet[1][0] = 1;
      tet.tet[0][1] = 1;
      tet.tet[1][1] = 1;
      tet.tet[2][1] = 1;
  }
  return tet;
}

/**
 * @brief Обработка ввода пользователя
 * @param[in] user_input Символ пользователя
 * @return 
 */
UserAction_t get_signal(int user_input) {
  UserAction_t rc = 0;

  if (user_input == KEY_UP)
    rc = Up;
  else if (user_input == KEY_DOWN)
    rc = Down;
  else if (user_input == KEY_LEFT)
    rc = Left;
  else if (user_input == KEY_RIGHT)
    rc = Right;
  else if (user_input == 'q' || user_input == 'Q')
    rc = Terminate;
  else if (user_input == 'p' || user_input == 'P')
    rc = Pause;
  else if (user_input == '\n')
    rc = Start;
  else if (user_input == ' ')
    rc = Action;

  return rc;
}

/**
 * @ingroup state_funcs
 * @brief Selection of the next function based on the game state
 * @param[in] *state Current game state
 * @param[in] action Human-readable signal from user
 */
void userInput(FSM_STATES_g *state, UserAction_t action) {
  switch (*state) {
    case START:
      start_state(state, action);
      break;
    case SPAWN:
      spawn_state(state);
      break;
    case MOVING:
      moving_state(state, action);
      break;
    case ATTACHING:
      attaching_state();
      *state = SPAWN;
      break;
    case GAME_OVER:
      *state = GAME_OVER;
      game_over(state, action);
      break;
    case PAUSE:
      pause_game(state, action);
      break;
    case EXIT_STATE:
      break;
  }
}

/**
 * @ingroup fsm_funcs
 * @brief Game over state
 * @param[in] *state Current game state
 * @param[in] sig Human-readable signal from user
 */
void game_over(FSM_STATES_g *state, UserAction_t sig) {
  if (sig == Start) {
    GameInfo_t *stats = updateCurrentState();
    stats_init(stats);
    clear();
    *state = SPAWN;
  } else if (sig == Terminate) {
    *state = EXIT_STATE;
  }
  save_score();
}

/**
 * @ingroup fsm_funcs
 * @brief Start state
 * @param[in] *state Current game state
 * @param[in] sig Human-readable signal from user
 */
void start_state(FSM_STATES_g *state, UserAction_t sig) {
  if (sig == Start)
    *state = SPAWN;
  else if (sig == Terminate)
    *state = EXIT_STATE;
}

/**
 * @ingroup fsm_funcs
 * @brief A game state that generates a new tetromino for the next turn
 * @param[in] *state Current game state
 */
void spawn_state(FSM_STATES_g *state) {
  GameInfo_t *stats = updateCurrentState();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      stats->current_tetromino.tet[i][j] = stats->next_tetromino.tet[i][j];
    }
  }
  stats->current_tetromino.type = stats->next_tetromino.type;
  stats->next_tetromino = get_tetromino(rand() % RAND);
  stats->cur_x = 4;
  stats->cur_y = 1;
  if (check_field(Down) != 0 && stats->current_tetromino.type != 0) {
    *state = GAME_OVER;
  } else if (stats->current_tetromino.type == 0) {
    if (check_field(Down) != 0) {
      rotate();
      if (stats->current_tetromino.tet[3][1] == 1) {
        stats->cur_y = 0;
        stats->cur_x = 3;
      }
    }
    if (check_field(Down) != 0)
      *state = GAME_OVER;
    else
      *state = MOVING;
  } else {
    *state = MOVING;
  }
}

/**
 * @ingroup fsm_funcs
 * @brief State of the game during tetromino movement
 * @param[in] *state Current game state
 * @param[in] sig Human-readable signal from user
 */
void moving_state(FSM_STATES_g *state, UserAction_t sig) {
  switch (sig) {
    case Action:
      rotate();
      break;
    case Left:
      move_left();
      break;
    case Right:
      move_right();
      break;
    case Down:
      move_down(state);
      break;
    case Terminate:
      *state = EXIT_STATE;
      break;
    case Pause:
      *state = PAUSE;
      pause_game(state, sig);
      break;
    default:
      break;
  }
}

/**
 * @ingroup fsm_funcs
 * @brief State of the game during tetromino attaching
 */
void attaching_state() {
  GameInfo_t *stats = updateCurrentState();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (stats->current_tetromino.tet[i][j] == 1) {
        stats->field[stats->cur_x + i][stats->cur_y + j - 1] = 1;
      }
    }
  }
  int row = clean_rows();
  if (row >= 1) {
    struct timespec ts;
    ts.tv_sec = 400 / 1000;
    ts.tv_nsec = (400 % 1000) * 1000000;
    nanosleep(&ts, NULL);
  } else {
    struct timespec ts;
    ts.tv_sec = 150 / 1000;
    ts.tv_nsec = (150 % 1000) * 1000000;
    nanosleep(&ts, NULL);
  }
  if (row == 1)
    stats->score += 100;
  else if (row == 2)
    stats->score += 300;
  else if (row == 3)
    stats->score += 700;
  else if (row >= 4)
    stats->score += 1500;
  stats->level = stats->score / 600;
  if (stats->level > 10) stats->level = 10;
  stats->speed = 700 - (stats->level * (stats->level > 5 ? 50 : 60));
  save_score();
}

/**
 * @ingroup check_funcs
 * @brief Clears filled rows
 * @return Returns the number of rows cleared
 */
int clean_rows() {
  GameInfo_t *stats = updateCurrentState();
  int row = 0;
  for (int j = 0; j < 20; j++) {
    int count = 0;
    for (int i = 0; i < 10; i++) {
      if (stats->field[i][j] == 1) {
        count++;
      }
    }
    if (count == 10) {
      for (int k = j; k > 0; k--) {
        for (int m = 0; m < 10; m++) {
          stats->field[m][k] = stats->field[m][k - 1];
        }
      }
      row++;
    }
  }
  return row;
}

/**
 * @ingroup move_funcs
 * @brief Moving left
 */
void move_left() {
  GameInfo_t *stats = updateCurrentState();
  int check = check_field(Left);
  if (!check) {
    stats->cur_x -= 1;
  }
}

/**
 * @ingroup move_funcs
 * @brief Moving right
 */
void move_right() {
  GameInfo_t *stats = updateCurrentState();
  int check = check_field(Right);
  if (!check) {
    stats->cur_x += 1;
  }
}

/**
 * @ingroup move_funcs
 * @brief Moving down
 */
void move_down(FSM_STATES_g *state) {
  GameInfo_t *stats = updateCurrentState();
  if (check_field(Down) == 0) {
    stats->cur_y += 1;
  } else {
    *state = ATTACHING;
  }
}

/**
 * @ingroup check_funcs
 * @brief Checking the field for collisions with the current tetromino
 * @param[in] sig Human-readable signal from user
 * @return Returns collision status
 */
int check_field(UserAction_t sig) {
  GameInfo_t *stats = updateCurrentState();
  int a = 0;
  int b = 0;
  if (sig == Left) {
    a = -1;
    b = -1;
  }
  if (sig == Right) {
    a = 1;
    b = -1;
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (stats->current_tetromino.tet[i][j] == 1) {
        int newX = stats->cur_x + i + a;
        int newY = stats->cur_y + j + b;
        if (newY < 0 || newY >= 20 || newX < 0 || newX > 9) {
          return 1;
        }
        if (stats->field[newX][newY] != 0) {
          return 1;
        }
      }
    }
  }
  return 0;
}

/**
 * @ingroup check_funcs
 * @brief Checking the field for collisions with the current tetromino during
 * rotation
 * @param[in] temp Rotated tetromino
 * @return Returns collision status
 */
int check_field_rotate(int temp[4][4]) {
  GameInfo_t *stats = updateCurrentState();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (temp[i][j] == 1) {
        int newX = stats->cur_x + i;
        int newY = stats->cur_y + j;
        if (newY < 0 || newY >= 20 || newX < 0 || newX > 9) {
          return 1;
        }
        if (stats->field[newX][newY] != 0) {
          return 1;
        }
      }
    }
  }
  return 0;
}

/**
 * @ingroup move_funcs
 * @brief Tetromino rotation
 */
void rotate() {
  GameInfo_t *stats = updateCurrentState();
  int size = 3;
  if (stats->current_tetromino.type == 0) {
    size = 4;
  }
  if (stats->current_tetromino.type == 1) {
    return;
  }
  int temp[4][4] = {0};
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (stats->current_tetromino.tet[i][j] == 1) temp[size - j - 1][i] = 1;
    }
  }
  if (check_field_rotate(temp) == 0) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (temp[i][j] == 1)
          stats->current_tetromino.tet[i][j] = 1;
        else
          stats->current_tetromino.tet[i][j] = 0;
      }
    }
  }
}

/**
 * @ingroup stats_funcs
 * @brief Stats initialization
 * @param[in] *stats Pointer to stats struct
 */
void stats_init(GameInfo_t *stats) {
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 22; j++) {
      stats->field[i][j] = 0;
    }
  }
  stats->next_tetromino = get_tetromino(rand() % RAND);
  stats->score = 0;
  stats->speed = 700;
  stats->level = 0;
  stats->cur_x = 4;
  stats->cur_y = 1;
  FILE *fp = fopen("score", "r");
  if (fp != NULL) {
    char temp[13] = "";
    fgets(temp, 12, fp);
    fclose(fp);
    stats->high_score = (int)strtol(temp, NULL, 10);
  }
}

/**
 * @ingroup stats_funcs
 * @brief Saving score to a file
 */
void save_score() {
  GameInfo_t *stats = updateCurrentState();
  if (stats->score > stats->high_score) {
    FILE *fp = fopen("score", "w");
    if (fp != NULL) {
      fprintf(fp, "%d", stats->score);
      fclose(fp);
    }
  }
}

/**
 * @ingroup fsm_funcs
 * @brief Game pause
 * @param[in] *state Current game state
 * @param[in] sig Human-readable signal from user
 */
void pause_game(FSM_STATES_g *state, UserAction_t sig) {
  GameInfo_t *stats = updateCurrentState();
  stats->pause += 1;
  if (sig == Terminate) {
    *state = EXIT_STATE;
  } else if (sig == Pause && stats->pause > 1) {
    *state = MOVING;
    stats->pause = 0;
  }
}

/**
 * @ingroup other_funcs
 * @brief Singleton function for stats update in any time
 * @return Returns the current game stats
 */
GameInfo_t *updateCurrentState() {
  static GameInfo_t state;
  static int initialized = 0;

  if (!initialized) {
    stats_init(&state);
    initialized = 1;
  }

  return &state;
}
