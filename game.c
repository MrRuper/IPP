/** @file
 * Implementation of the interface game.h
 *
 * @author Bogdan Petraszczuk <bp372955@students.mimuw.edu.pl>
 *                            <bogdan.petraszczuk@gmail.com>
 * @copyright Uniwersytet Warszawski
 * @date 2023
 */

#include "game.h"
#include <stdio.h>
#include <stdlib.h>

/** @brief An auxiliary structure which keeps the player number and the "color" of the field
 *  in the game_board (i.e. the number of calls of the game_move function).
 *  The maximum number of calls of the game_move function is the
 *  number of fields in game_board matrix (i.e. UINT64_MAX).
 */
typedef struct Pair{
    uint32_t player_number;
    uint64_t color;
}pair_t;

/** @brief This structure represents the player information:
 * busy_fields     - the non negative number of occupied fields by
 * the player,
 * busy_areas      - the non negative number of occupied aries by
 * the player (the precise definition of area could be found
 * in game.h file),
 * boundary_length - the non negative number which represents
 * the length of the "boundary" of that player. In other words
 * this is the number of free to take fields by the player if the
 * the Player.busy_areas = game.max_areas (i.e. the player took
 * already all possibles areas but still can play putting figures
 * on the "boundary" of already existing connected fragment marked
 * by his number),
 * player_symbol   - symbol describing the player on the game board.
 */
typedef struct Player{
    uint64_t busy_fields;
    uint64_t busy_areas;
    uint64_t boundary_length;
    const char player_symbol;
}player_t;

/** @brief This structure represents the whole game.
 * width - non negative number which describes the width of the game board,
 * height - non negative number which describes the height of the game board,
 * number_of_players - non negative number representing the number of players,
 * max_areas - non negative number representing the maximum of free to take
 * areas by each of the player,
 * game_board - the matrix representing game status,
 * all_players - the array of players. The number of the player is his index
 * in that array,
 * neighbors - helper array which holds the neighbors number and color
 * of some figure on game_board,
 * diff_neighbour_number - helper array which is the same as neighbours parameter
 * but holds only numbers of the neighbours,
 * fields_to_take - non negative number of free fields in the game_board.
 */
struct game{
    //HOW TO DO CONST IN NONSTATIC MEMBER?
    uint32_t width;
    uint32_t height;
    uint32_t number_of_players;
    uint64_t max_areas;
    pair_t** game_board;
    player_t* all_players;
    pair_t* neighbours;
    uint32_t* diff_neighbour_number;
    uint64_t fields_to_take;
};

uint64_t global_counter = 1;

/** Standard min function.
 * @param[in] x - first number,
 * @param[in] y - second number,
 * @return minimum of x and y.
 */
static uint64_t min (uint64_t const x, uint64_t const y) {
    if (x < y){
        return x;
    }

    return y;
}

game_t* game_new(uint32_t width, uint32_t height, uint32_t players, uint32_t areas){
    /**
     * firstly check if the input is correct
     */
    if (width == 0 || height == 0 || players == 0 || areas == 0){
        return NULL;
    }

    /**
     * malloc / calloc all needed stuff for further work
     */
    game_t* g = (game_t*)malloc(sizeof(game_t));
    player_t* all_players = calloc(players, sizeof(player_t));
    pair_t* neighbours = calloc(4, sizeof(pair_t));
    uint32_t* diff_neighbour_number = calloc (4, sizeof(uint32_t));
    if (!g || !all_players || !neighbours || diff_neighbour_number){
        return NULL;
    }

    /**
     * calloc the matrix
     */
    pair_t** game_board = calloc(width, sizeof(pair_t*));

    if (!game_board){
        return NULL;
    }

    for (uint32_t i = 0; i < height; i++){
        game_board[i] = calloc(height, sizeof(pair_t));

        if (!game_board[i]){
            return NULL;
        }
    }

    /**
     * the game creating
     */
    g->width = width;
    g->height = height;
    g->number_of_players = players;
    g->max_areas = areas;
    g->game_board = game_board;
    g->all_players = all_players;
    g->neighbours = neighbours;
    g->diff_neighbour_number = diff_neighbour_number;
    g->fields_to_take = width * height;

    return g;
}

void game_delete(game_t *g){
    if (!g){
        free(g->neighbours);
        free(g->all_players);
        free(g->diff_neighbour_number);

        for (uint32_t i = 0; i < g->width; i++){
            free(g->game_board[i]);
        }

        free(g->game_board);
        free(g);
    }
}

/** @brief This function verifies if the player number is correct.
 *  Here we assume that the parameter g is not null.
 * @param[in] g               - pointer to the game structure,
 * @param[in] player_number   - number of the player,
 * @return true if the player number is correct and false otherwise.
 */
static bool correct_player_number(game_t const* g, uint32_t const player_number){
    if (player_number == 0 || player_number > g->number_of_players){
        return false;
    }

    return true;
}

/** @brief This function verifies if the player occupies already all possible
 *  areas.
 * @param[in] g               - pointer to the game structure,
 * @param[in] player_number   - number of the player,
 * @return true if player has no free areas to take and false otherwise.
 */
static bool player_took_all_areas(game_t const* g, uint32_t const player_number){
    if (g->all_players[player_number - 1].busy_areas == g->max_areas){
        return true;
    }

    return false;
}

/** @brief This function verifies the validity of coordinate (x,y).
 * @param[in] g - game structure pointer,
 * @param[in] x - column number,
 * @param[in] y - row number,
 * @return true if the coordinate (x,y) is valid (i.e. belongs to the
 * game board) and false otherwise.
 */
static bool correct_coordinate(game_t const* g, uint32_t const x, uint32_t const y){
    if (x >= g->width || y >= g->height){
        return false;
    }

    return true;
}

/** @brief This function checks if the field is empty.
 * @param g  - pointer to the game structure,
 * @param x  - column number,
 * @param y  - row number,
 * @return true if coordinate (x,y) is empty and false
 * otherwise.
 */
static bool empty_coordinate (game_t const* g, uint32_t const x, uint32_t const y){
    if (g->game_board[x][y].player_number == 0){
        return true;
    }

    return false;
}

/** @brief This is an additional function which adds the new pair
 * to the game.neighbors. If the copy is found the function is terminated.
 * @param[in] position       - technical parameter describing the number
 *                         of already added pairs,
 * @param[in] neighbours     -
 * @param[in] value_to_add
 */
static void add_to_array (int* position, pair_t* neighbours, pair_t const value_to_add){
    for (int i = 0; i < *position; i++){
        if (neighbours[i].player_number == value_to_add.player_number &&
                neighbours[i].color == value_to_add.color){
            return;
        }
    }

    neighbours[*position].player_number = value_to_add.player_number;
    neighbours[*position].color = value_to_add.color;
    (*position)++;
}

/** @brief modify game.neighbors to get all different direct
 *  neighbours of the coordinate (x,y). Value {0,0} denotes
 *  empty field.
 * @param[in] g  - pointer to the game structure,
 * @param x      - column number,
 * @param y      - row number.
 */
static void different_direct_neighbours_pair(game_t const* g, uint32_t const x, uint32_t const y){
    int position = 0;
    bool valid_right = correct_coordinate(g, x + 1, y);
    bool valid_left = correct_coordinate(g, x - 1, y);
    bool valid_down = correct_coordinate(g, x, y + 1);
    bool valid_up = correct_coordinate(g, x, y - 1);

    if (valid_right && !empty_coordinate(g, x, y)){
        add_to_array(&position, g->neighbours, g->game_board[x + 1][y]);
    }
    if (valid_left){
        add_to_array(&position, g->neighbours, g->game_board[x - 1][y]);
    }
    if (valid_down){
        add_to_array(&position, g->neighbours, g->game_board[x][y + 1]);
    }
    if (valid_up){
        add_to_array(&position, g->neighbours, g->game_board[x][y - 1]);
    }
}

/** @brief This function is a technical function for getting different player numbers
 * from already filled neighbour array.
 * @param g - pointer to the game structure.
 */
static void diff_neighbour_numbers(game_t const* g){
    int i = 0;
    int j = 0;

    while (i < 4 && g->neighbours[i].player_number != 0){
        for (int z = 0; z < j; z++) {
            if (g->neighbours[i].player_number == g->diff_neighbour_number[z]) {
                i++;
                continue;
            }
        }
            g->diff_neighbour_number[j] = g->neighbours[i].player_number;
            j++;
            i++;
    }
}

// returns the number of different neighbours
static uint64_t number_of_different_neighbours (game_t const* g){
    uint64_t i = 0;

    while (i < 4 && g->diff_neighbour_number[i] != 0){
        i++;
    }

    return i;
}

//returns the number of busy neighbours
static uint64_t number_of_neighbors (game_t const* g){
    uint64_t answer = 0;

    for (int i = 0; i < 4; i++){
        if (g->neighbours[i].player_number != 0){
            answer++;
        }
    }

    return answer;
}

/** @brief This function checks if the player_number has himself in the
 *  neighbour array. In other words we check whether the game_move function
 *  creates the new area for the player or just stick to another fragment.
 * @param neighbours      - is the array of neighbours of the player,
 * @param player_number   - the player number
 * @return true if the array neighbours has player_number inside.
 */
static bool boundary_adding(pair_t const* neighbours, uint32_t const player_number){
    int i = 0;

    while (neighbours[i].player_number != 0 && i < 4){
        if (neighbours[i].player_number == player_number){
            return true;
        }
        i++;
    }

    return false;
}

/** @brief This function finds all empty neighbours of the player on position (x,y) and
 *  checks if the neighbour of the found empty field has the same number as our player.
 *  In other words this function returns the number of free fields neighbours of (x,y) which
 *  could not be added to a boundary length of the player on that position.
 * @param g               - pointer to the game structure,
 * @param x               - column number,
 * @param y               - row number,
 * @param player_number   - the number of the figure we put at (x,y) coordinate.
 * @return The number of empty neighbours of the (x,y) coordinate which has
 * in their own neighbours the player_number.
 */
static uint64_t check_non_direct_neighbours (game_t const* g, uint32_t x,
                                        uint32_t y, uint32_t player_number){
    uint64_t answer = 0;
    bool valid_left = correct_coordinate(g, x - 1, y);
    bool valid_right = correct_coordinate(g, x + 1, y);
    bool valid_up = correct_coordinate(g, x, y - 1);
    bool valid_down = correct_coordinate(g, x, y + 1);

    //technical bool variables
    bool technical1;
    bool technical2;
    bool technical3;

    if (valid_left && empty_coordinate(g, x - 1, y)){
        technical1 = correct_coordinate(g, x - 2, y);
        technical2 = correct_coordinate(g, x - 1, y + 1);
        technical3 = correct_coordinate(g, x - 1, y - 1);

        if (technical1 && g->game_board[x - 2][y].player_number == player_number ||
                technical2 && g->game_board[x - 1][y + 1].player_number == player_number ||
                 technical3 && g->game_board[x - 1][y - 1].player_number == player_number){
            answer++;
        }
    }
    if (valid_right && empty_coordinate(g, x + 1, y)){
        technical1 = correct_coordinate(g, x + 2, y);
        technical2 = correct_coordinate(g, x + 1, y - 1);
        technical3 = correct_coordinate(g, x + 1, y + 1);

        if (technical1 && g->game_board[x + 2][y].player_number == player_number ||
                technical2 && g->game_board[x + 1][y - 1].player_number == player_number ||
                 technical3 && g->game_board[x + 1][y + 1].player_number == player_number){
            answer++;
        }
    }
    if (valid_up && empty_coordinate(g, x, y - 1)){
        technical1 = correct_coordinate(g, x, y - 2);
        technical2 = correct_coordinate(g, x - 1, y - 1);
        technical3 = correct_coordinate(g, x + 1, y - 1);

        if (technical1 && g->game_board[x][y - 2].player_number == player_number ||
                technical2 && g->game_board[x - 1][y - 1].player_number == player_number ||
                 technical3 && g->game_board[x + 1][y - 1].player_number == player_number){
            answer++;
        }
    }
    if (valid_down && empty_coordinate(g, x, y + 1)){
        technical1 = correct_coordinate(g, x, y + 2);
        technical2 = correct_coordinate(g, x - 1, y + 1);
        technical3 = correct_coordinate(g, x + 1, y + 1);

        if (technical1 && g->game_board[x][y + 2].player_number == player_number ||
                technical2 && g->game_board[x - 1][y + 1].player_number == player_number ||
                 technical3 && g->game_board[x + 1][y + 1].player_number == player_number){
            answer++;
        }
    }

    return answer;
}

/** @brief This procedure will reset neighbour and diff_neighbour_number
 * members to zero.
 * @param g - pointer on game structure.
 */
static void set_to_zero (game_t* g){
    for (int i = 0; i < 4; i++){
        g->neighbours[i].player_number = 0;
        g->neighbours[i].color = 0;
        g->diff_neighbour_number[i] = 0;
    }
}

/** @brief This function will find the minimum color in all direct
 * neighbours of player_number with (x,y) coordinate. Later on all
 * neighbour fragments with the same number will be (using DFS)
 * recolor on the min color.
 * @param[in] g               - pointer on the game structure,
 * @param[in] x               - column number,
 * @param[in] y               - row number,
 * @param[in] player_number       - player number.
 * @return Min color of all direct neighbours with the same player_number.
 */
static uint64_t find_min_color(game_t const* g, uint32_t x, uint32_t y, uint32_t player_number){
    uint64_t answer = UINT64_MAX;

    for (int i = 0; i < 4; i++){
        if (g->neighbours[i].player_number == player_number){
            answer = min(answer, g->neighbours[i].color);
        }
    }

    return answer;
}

bool game_move(game_t* g, uint32_t player, uint32_t x, uint32_t y){
    uint64_t number_of_diff_neighbours;
    uint64_t number_of_neighbors; //not necessarily different

    /**
     * firstly verify the input
     */
     if (!g || !correct_player_number(g, player) || !correct_coordinate(g, x, y)){
         return false;
     }

     /**
      * The field is already busy
      */
      if (g->game_board[x][y].player_number != 0){
          return false;
      }

      /**
       * Here the move is legal. We split next part of that
       * function on two cases:
       * (1) the move is "boundary" i.e. we just add the figure
       * of the player to some area owned by the same player,
       * (2) the move creates new area meaning there is no
       * neighbours of (x,y) coordinate marked with number
       * "player".
       */
    different_direct_neighbours_pair(g, x, y);
    diff_neighbour_numbers(g);
    number_of_diff_neighbours = number_of_different_neighbours(g);
    number_of_neighbors = number_of_different_neighbours(g);

    if (!boundary_adding(g->neighbours, player)){
        /**
         * The player can not create new area
         */
        if (player_took_all_areas(g, player)){
            return false;
        }

        /**
         * Firstly update me
         */
        g->all_players[player - 1].busy_areas++;
        g->all_players[player - 1].busy_fields++;
        g->all_players[player - 1].boundary_length += 4 - number_of_neighbors
                                                        - check_non_direct_neighbours(g, x, y, player);

        /**
         * Update the game structure and the global_counter
         */
         g->game_board[x][y].player_number = player;
         g->game_board[x][y].color = global_counter;
         global_counter++;
         g->fields_to_take--;

         /**
          * Update all non empty neighbours
          */
          uint32_t helper_variable;

         for (int i = 0; i < number_of_diff_neighbours; i++){
             helper_variable = g->diff_neighbour_number[i];
             g->all_players[helper_variable - 1].boundary_length--;
         }

        set_to_zero(g);

        return true;
    }
    else {
        uint64_t min_color = find_min_color(g, x, y, player);
        uint64_t fragments = 0;
        bool valid_right = correct_coordinate(g, x + 1, y);
        bool valid_left = correct_coordinate(g, x - 1, y);
        bool valid_up = correct_coordinate(g, x, y - 1);
        bool valid_down = correct_coordinate(g, x, y + 1);

        /**
         * Firstly find the number of the same neighbours
         */
         for (int i = 0; i < 4; i++){
             if (g->neighbours[i].player_number == player){
                 fragments++;
             }
         }

        /**
         * Update me
         */
        g->all_players[player - 1].busy_areas -= fragments - 1;
        g->all_players[player - 1].busy_fields++;
        g->all_players[player - 1].boundary_length += 4 - number_of_neighbors
                                                      - check_non_direct_neighbours(g, x, y, player);

        /**
         * Update the game structure and the global_counter
         */
        g->game_board[x][y].player_number = player;
        g->game_board[x][y].color = min_color;
        global_counter++;


        /**
         * Update all neighbours with different figures
         */
         int local_index = 0;
         uint32_t local_number;

         while (local_index < 4 && g->diff_neighbour_number[local_index] != 0){
             local_number = g->diff_neighbour_number[local_index];

             if (local_number != player){
                g->all_players[local_number - 1].boundary_length--;
             }

             local_index++;
         }

         /**
          * Update all neighbours with the same number recoloring them
          */
          

    }

}

uint64_t game_busy_fields(game_t const* g, uint32_t player){
    if (!g || !correct_player_number(g, player)){
        return 0;
    }

    return g->all_players[player - 1].busy_fields;
}

uint64_t game_free_fields(game_t const* g, uint32_t player){
    if (!g || correct_player_number(g, player)){
        return 0;
    }
    if (player_took_all_areas(g, player)){
        return g->all_players[player - 1].boundary_length;
    }

    return g->fields_to_take;
}

uint32_t game_board_width(game_t const *g){
    if (!g){
        return 0;
    }

    return g->width;
}

uint32_t game_board_height(game_t const *g){
    if (!g){
        return 0;
    }

    return g->height;
}

uint32_t game_players(game_t const *g){
    if (!g){
        return 0;
    }

    return g->number_of_players;
}

char game_player(game_t const* g, uint32_t player){
    if (!g || !correct_player_number(g, player)){
        return ' ';
    }

    return g->all_players[player - 1].player_symbol;
}

//TO DO
char* game_board(game_t const *g){
    return NULL;
}
