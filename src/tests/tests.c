#include <check.h>
#include <stdio.h>

#include "../tetris/tetris.h"

START_TEST(start_test_1) {
  FSM_STATES_g state = START;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  userInput(&state, Start);
  ck_assert_int_eq(state, SPAWN);
}
END_TEST

START_TEST(start_test_2) {
  FSM_STATES_g state = START;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  userInput(&state, Up);
  ck_assert_int_eq(state, START);
  userInput(&state, Terminate);
  ck_assert_int_eq(state, EXIT_STATE);
}
END_TEST

START_TEST(spawn_test) {
  FSM_STATES_g state = SPAWN;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->next_tetromino = get_tetromino(0);
  userInput(&state, Start);
  ck_assert_int_eq(stats->current_tetromino.type, 0);
}
END_TEST

START_TEST(game_over_test_1) {
  FSM_STATES_g state = SPAWN;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->next_tetromino = get_tetromino(0);
  stats->field[5][1] = 1;
  userInput(&state, Start);
  ck_assert_int_eq(state, GAME_OVER);
  userInput(&state, Terminate);
  ck_assert_int_eq(state, EXIT_STATE);
}
END_TEST

START_TEST(game_over_test_2) {
  FSM_STATES_g state = SPAWN;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->current_tetromino = get_tetromino(1);
  stats->field[4][1] = 1;
  userInput(&state, Start);
  ck_assert_int_eq(state, GAME_OVER);
  userInput(&state, Start);
  ck_assert_int_eq(state, SPAWN);
}
END_TEST

START_TEST(moving_test_1) {
  FSM_STATES_g state = SPAWN;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  int prev_x = 0, prev_y = 0;
  stats->next_tetromino = get_tetromino(2);
  userInput(&state, Start);
  ck_assert_int_eq(state, MOVING);
  prev_x = stats->cur_x;
  prev_y = stats->cur_y;
  userInput(&state, Down);
  ck_assert_int_eq(prev_y + 1, stats->cur_y);
  prev_x = stats->cur_x;
  prev_y = stats->cur_y;
  userInput(&state, Left);
  ck_assert_int_eq(prev_x - 1, stats->cur_x);
  prev_x = stats->cur_x;
  prev_y = stats->cur_y;
  userInput(&state, Right);
  ck_assert_int_eq(prev_x + 1, stats->cur_x);
  userInput(&state, Action);
  ck_assert_int_eq(stats->current_tetromino.tet[2][0], 1);
}
END_TEST

START_TEST(moving_test_2) {
  FSM_STATES_g state = SPAWN;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  int prev_x = 0;
  stats->next_tetromino = get_tetromino(1);
  userInput(&state, Start);
  ck_assert_int_eq(state, MOVING);
  prev_x = stats->cur_x;
  userInput(&state, Action);
  ck_assert_int_eq(prev_x, stats->cur_x);
  userInput(&state, Up);
  ck_assert_int_eq(state, MOVING);
  userInput(&state, Pause);
  ck_assert_int_eq(state, PAUSE);
  userInput(&state, Pause);
  userInput(&state, Terminate);
  ck_assert_int_eq(state, EXIT_STATE);
}
END_TEST

START_TEST(attaching_test_1) {
  FSM_STATES_g state = MOVING;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->current_tetromino = get_tetromino(0);
  for (int i = 1; i < 10; i++) {
    stats->field[i][19] = 1;
    stats->field[i][18] = 1;
    stats->field[i][17] = 1;
    stats->field[i][16] = 1;
  }
  stats->cur_x = -1;
  stats->cur_y = 17;
  userInput(&state, Down);
  ck_assert_int_eq(state, ATTACHING);
  userInput(&state, Down);
  ck_assert_int_eq(state, SPAWN);
  ck_assert_int_eq(stats->score, 1500);
  ck_assert_int_eq(stats->level, 2);
}
END_TEST

START_TEST(attaching_test_2) {
  FSM_STATES_g state = MOVING;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->current_tetromino = get_tetromino(0);
  stats->cur_x = 4;
  stats->cur_y = 17;
  userInput(&state, Down);
  ck_assert_int_eq(state, ATTACHING);
  userInput(&state, Down);
  ck_assert_int_eq(state, SPAWN);
  ck_assert_int_eq(stats->score, 0);
}
END_TEST

START_TEST(pause_test) {
  FSM_STATES_g state = MOVING;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->current_tetromino = get_tetromino(0);
  userInput(&state, Pause);
  ck_assert_int_eq(state, PAUSE);
  userInput(&state, Pause);
  ck_assert_int_eq(state, MOVING);
  userInput(&state, Pause);
  ck_assert_int_eq(state, PAUSE);
  userInput(&state, Terminate);
  ck_assert_int_eq(state, EXIT_STATE);
}
END_TEST

START_TEST(get_signal_test) {
  UserAction_t result = 0;
  result = get_signal(0403);
  ck_assert_int_eq(result, Up);
  result = get_signal(0402);
  ck_assert_int_eq(result, Down);
  result = get_signal(0404);
  ck_assert_int_eq(result, Left);
  result = get_signal(0405);
  ck_assert_int_eq(result, Right);
  result = get_signal('q');
  ck_assert_int_eq(result, Terminate);
  result = get_signal('p');
  ck_assert_int_eq(result, Pause);
  result = get_signal('\n');
  ck_assert_int_eq(result, Start);
  result = get_signal(' ');
  ck_assert_int_eq(result, Action);
}
END_TEST

START_TEST(userInput_test) {
  FSM_STATES_g state = START;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  userInput(&state, Up);
  ck_assert_int_eq(state, START);
  state = SPAWN;
  userInput(&state, Up);
  ck_assert_int_eq(state, MOVING);
  state = MOVING;
  userInput(&state, Up);
  ck_assert_int_eq(state, MOVING);
  state = ATTACHING;
  userInput(&state, Up);
  ck_assert_int_eq(state, SPAWN);
  state = GAME_OVER;
  userInput(&state, Up);
  ck_assert_int_eq(state, GAME_OVER);
  state = PAUSE;
  userInput(&state, Up);
  ck_assert_int_eq(state, PAUSE);
  state = EXIT_STATE;
  userInput(&state, Up);
  ck_assert_int_eq(state, EXIT_STATE);
}
END_TEST

START_TEST(score_input_output_test) {
  FILE *fp = fopen("score", "w");
  if (fp != NULL) {
    fprintf(fp, "%d", 499);
    fclose(fp);
  }
  FSM_STATES_g state = START;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  ck_assert_int_eq(stats->high_score, 499);
  stats->score = 500;
  state = GAME_OVER;
  userInput(&state, Up);
  fp = fopen("score", "r");
  int result = 0;
  if (fp != NULL) {
    char temp[5] = "";
    fgets(temp, 4, fp);
    fclose(fp);
    result = (int)strtol(temp, NULL, 10);
  }
  ck_assert_int_eq(result, 500);
}
END_TEST

START_TEST(get_tetromino_test) {
  tetromino result = {0};
  result = get_tetromino(3);
  int tet3[4][4] = {0};
  tet3[2][0] = 1;
  tet3[0][1] = 1;
  tet3[1][1] = 1;
  tet3[2][1] = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(result.tet[i][j], tet3[i][j]);
    }
  }
  result = get_tetromino(6);
  int tet6[4][4] = {0};
  tet6[1][0] = 1;
  tet6[0][1] = 1;
  tet6[1][1] = 1;
  tet6[2][1] = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(result.tet[i][j], tet6[i][j]);
    }
  }
}
END_TEST

START_TEST(score_test_1) {
  FSM_STATES_g state = MOVING;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->current_tetromino = get_tetromino(0);
  for (int i = 1; i < 10; i++) {
    stats->field[i][19] = 1;
    stats->field[i][18] = 1;
    stats->field[i][17] = 1;
  }
  stats->cur_x = -1;
  stats->cur_y = 17;
  userInput(&state, Down);
  ck_assert_int_eq(state, ATTACHING);
  userInput(&state, Down);
  ck_assert_int_eq(state, SPAWN);
  ck_assert_int_eq(stats->score, 700);
  ck_assert_int_eq(stats->level, 1);
}
END_TEST

START_TEST(score_test_2) {
  FSM_STATES_g state = MOVING;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->current_tetromino = get_tetromino(0);
  for (int i = 1; i < 10; i++) {
    stats->field[i][19] = 1;
    stats->field[i][18] = 1;
  }
  stats->cur_x = -1;
  stats->cur_y = 17;
  userInput(&state, Down);
  ck_assert_int_eq(state, ATTACHING);
  userInput(&state, Down);
  ck_assert_int_eq(state, SPAWN);
  ck_assert_int_eq(stats->score, 300);
  ck_assert_int_eq(stats->level, 0);
}
END_TEST

START_TEST(score_test_3) {
  FSM_STATES_g state = MOVING;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->current_tetromino = get_tetromino(0);
  for (int i = 1; i < 10; i++) {
    stats->field[i][19] = 1;
  }
  stats->cur_x = -1;
  stats->cur_y = 17;
  userInput(&state, Down);
  ck_assert_int_eq(state, ATTACHING);
  userInput(&state, Down);
  ck_assert_int_eq(state, SPAWN);
  ck_assert_int_eq(stats->score, 100);
  ck_assert_int_eq(stats->level, 0);
}
END_TEST

START_TEST(check_collision_test) {
  FSM_STATES_g state = MOVING;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->current_tetromino = get_tetromino(0);
  stats->cur_x = -1;
  stats->cur_y = 3;
  userInput(&state, Left);
  ck_assert_int_eq(state, MOVING);
  ck_assert_int_eq(stats->cur_x, -1);
  stats->cur_x = 8;
  stats->cur_y = 3;
  userInput(&state, Right);
  ck_assert_int_eq(state, MOVING);
  ck_assert_int_eq(stats->cur_x, 8);
  stats->cur_x = 8;
  stats->cur_y = 17;
  userInput(&state, Down);
  ck_assert_int_eq(state, ATTACHING);
  ck_assert_int_eq(stats->cur_y, 17);
}
END_TEST

START_TEST(check_collision_r_test) {
  FSM_STATES_g state = MOVING;
  GameInfo_t *stats = updateCurrentState();
  stats_init(stats);
  stats->current_tetromino = get_tetromino(0);
  stats->cur_x = -1;
  stats->cur_y = 3;
  userInput(&state, Action);
  ck_assert_int_eq(state, MOVING);
  stats->cur_x = 8;
  stats->cur_y = 3;
  userInput(&state, Action);
  ck_assert_int_eq(state, MOVING);
  stats->cur_x = 8;
  stats->cur_y = 16;
  userInput(&state, Action);
  ck_assert_int_eq(state, MOVING);
  stats->field[5][10] = 1;
  stats->cur_x = 4;
  stats->cur_y = 9;
  userInput(&state, Action);
  ck_assert_int_eq(state, MOVING);
}
END_TEST

void srunner_state_funcs(SRunner *sr) {
  Suite *Suite1 = suite_create("state");
  TCase *TestCase1 = tcase_create("state");
  suite_add_tcase(Suite1, TestCase1);

  tcase_add_test(TestCase1, start_test_1);
  tcase_add_test(TestCase1, start_test_2);

  tcase_add_test(TestCase1, spawn_test);

  tcase_add_test(TestCase1, game_over_test_1);
  tcase_add_test(TestCase1, game_over_test_2);

  tcase_add_test(TestCase1, moving_test_1);
  tcase_add_test(TestCase1, moving_test_2);

  tcase_add_test(TestCase1, attaching_test_1);
  tcase_add_test(TestCase1, attaching_test_2);

  tcase_add_test(TestCase1, pause_test);

  srunner_add_suite(sr, Suite1);
}

void srunner_input_funcs(SRunner *sr) {
  Suite *Suite2 = suite_create("input");
  TCase *TestCase2 = tcase_create("input");
  suite_add_tcase(Suite2, TestCase2);

  tcase_add_test(TestCase2, get_signal_test);

  tcase_add_test(TestCase2, userInput_test);

  tcase_add_test(TestCase2, score_input_output_test);

  srunner_add_suite(sr, Suite2);
}

void srunner_other_funcs(SRunner *sr) {
  Suite *Suite3 = suite_create("other");
  TCase *TestCase3 = tcase_create("other");
  suite_add_tcase(Suite3, TestCase3);

  tcase_add_test(TestCase3, get_tetromino_test);

  tcase_add_test(TestCase3, score_test_1);
  tcase_add_test(TestCase3, score_test_2);
  tcase_add_test(TestCase3, score_test_3);

  tcase_add_test(TestCase3, check_collision_test);
  tcase_add_test(TestCase3, check_collision_r_test);

  srunner_add_suite(sr, Suite3);
}

int main() {
  Suite *s = suite_create("s21_matrix");
  SRunner *sr = srunner_create(s);

  srunner_state_funcs(sr);
  srunner_input_funcs(sr);
  srunner_other_funcs(sr);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  int fails = srunner_ntests_failed(sr);
  srunner_free(sr);
  return fails;
}