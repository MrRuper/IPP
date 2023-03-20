game_t *g;
//test1: male dane
g = game_new(1, 11, UINT32_MAX, 0);
assert(g == NULL);
assert(game_board_width(g) == 0);
g = game_new(SIZE_MAX, SIZE_MAX, UINT32_MAX, UINT32_MAX);
assert(g == NULL);

g = game_new(3, 3, 2, 4);
assert(g != NULL);
assert(game_board_height(g) == 3);
assert(game_board_width(g) == 3);
assert(game_players(g) == 2);
assert(game_move(g, 1, 0, 0));
assert(!game_move(g, 2, 0, 0));
assert(!game_move(g, 3, 1, 1));
assert(!game_move(g, 1, 0, 0));
assert(game_busy_fields(g, 1) == 1);
assert(game_busy_fields(g, 2) == 0);
assert(game_free_fields(g, 1) == 8);
assert(game_free_fields(g, 2) == 8);
assert(game_move(g, 1, 2, 0));
assert(game_busy_fields(g, 1) == 2);
assert(game_busy_fields(g, 2) == 0);
assert(game_free_fields(g, 1) == 7);
assert(game_free_fields(g, 2) == 7);
assert(game_free_fields(g, 12) == 0);
assert(game_move(g, 1, 0, 2));
assert(game_move(g, 1, 2, 2));
assert(game_busy_fields(g, 1) == 4);
assert(game_free_fields(g, 1) == 4);
assert(!game_move(g, 1, 1, 1));
assert(game_move(g, 2, 1, 0));
assert(game_free_fields(g, 1) == 3);
assert(game_free_fields(g, 2) == 4);
assert(!game_move(g, 2, 0, 0));
assert(game_move(g, 2, 0, 1));
assert(game_move(g, 2, 1, 2));
assert(game_free_fields(g, 1) == 1);
assert(game_free_fields(g, 2) == 2);
assert(!game_move(g, 1, 1, 1));
assert(game_move(g, 2, 2, 1));
assert(game_free_fields(g, 1) == 0);
assert(game_free_fields(g, 2) == 1);
assert(game_busy_fields(g, 2) == 4);
assert(game_move(g, 2, 1, 1));
assert(game_free_fields(g, 2) == 0);

game_delete(g);


//test2 duze dane
