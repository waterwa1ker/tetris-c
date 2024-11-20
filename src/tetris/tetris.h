/**
 * @file tetris.h
 * @brief Main header file
 * @author nataliak
 */

#ifndef TETRIS_H
#define TETRIS_H
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/// The number of tetrominos we want to use in the game, 7 in total
#define RAND 7

/**
 * @brief FSM Definition
 */
typedef enum {
  START,
  SPAWN,
  MOVING,
  PAUSE,
  ATTACHING,
  GAME_OVER,
  EXIT_STATE
} FSM_STATES_g;

/**
 * @brief Definition of the user's signals
 */
typedef enum {
  Start = 1,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/**
 * @brief Tetromino struct
 */
typedef struct {
  /// @brief Tetromino type
  int type;
  /// @brief Array with tetromino
  int tet[4][4];
} tetromino;

/**
 * @brief Structure containing game stats
 */
typedef struct {
  /// @brief A tetromino field
  int field[12][22];
  /// @brief Next tetromino
  tetromino next_tetromino;
  /// @brief Current tetromino
  tetromino current_tetromino;
  /// @brief Score
  int score;
  /// @brief High score (from file)
  int high_score;
  /// @brief Level
  int level;
  /// @brief Speed
  int speed;
  /// @brief Is the game paused
  int pause;
  /// @brief Position of the current tetromino at X
  int cur_x;
  /// @brief Position of the current tetromino at Y
  int cur_y;
} GameInfo_t;

/**
 * @defgroup state_funcs State control and signal processing
 */
void userInput(FSM_STATES_g *state, UserAction_t sig);
UserAction_t get_signal(int user_input);

/**
 * @defgroup other_funcs Other
 */
GameInfo_t *updateCurrentState();
tetromino get_tetromino(int num);
void game_loop();

/**
 * @defgroup fsm_funcs Functions of a finite automaton
 */
void start_state(FSM_STATES_g *state, UserAction_t sig);
void spawn_state(FSM_STATES_g *state);
void moving_state(FSM_STATES_g *state, UserAction_t sig);
void attaching_state();
void pause_game(FSM_STATES_g *state, UserAction_t sig);
void game_over(FSM_STATES_g *state, UserAction_t sig);

/**
 * @defgroup stats_funcs Statistics and score management
 */
void stats_init(GameInfo_t *stats);
void save_score();

/**
 * @defgroup graphics_funcs GUI and graphics
 */
void print_tetromino();
void print_game();
void print_field();
void print_banner();
void print_start_banner();
void print_something(FSM_STATES_g state);
void clear_field();
void clear_info();

/**
 * @defgroup move_funcs Tetromino controls
 */
void move_left();
void move_right();
void move_down(FSM_STATES_g *state);
void rotate();

/**
 * @defgroup check_funcs Collision checks
 */
int check_field(UserAction_t sig);
int check_field_rotate(int temp[4][4]);
int clean_rows();

#endif /* TETRIS_H */
