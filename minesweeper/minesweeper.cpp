#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "minesweeper.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {
  cout << "Loading board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in) cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer, 512);
  while (in && row < 9) {
    for (int n = 0; n < 9; n++) {
      assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' ||
             buffer[n] == '?' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer, 512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
  cout << (char)('A' + row) << "|";
  for (int i = 0; i < 9; i++) cout << ((data[i] == '.') ? ' ' : data[i]);
  cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
  cout << "  ";
  for (int r = 0; r < 9; r++) cout << (char)('1' + r);
  cout << endl;
  cout << " +---------+" << endl;
  for (int r = 0; r < 9; r++) print_row(board[r], r);
  cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */
void initialise_board(char board[9][9]) {
  for (int r = 0; r < 9; r++)
    for (int c = 0; c < 9; c++) board[r][c] = '?';
}

/* add your functions here */

bool is_complete(char mines[9][9], char revealed[9][9]) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (mines[i][j] == '.' && revealed[i][j] == '?') return false;
    }
  }
  return true;
}

int count_mines(const string position, char mines[9][9]) {
  // check legal position
  if (position.length() != 2) return -1;
  if (position[0] < 'A' || position[0] > 'I') return -1;
  if (position[1] < '1' || position[1] > '9') return -1;

  int new_position_x, new_position_y;
  int count = 0;

  // count mines
  int direction[8][2] = {{1, 0},  {1, 1},   {0, 1},  {-1, 1},
                         {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

  for (int i = 0; i < 8; i++) {
    new_position_x = position[0] - 'A' + direction[i][0];
    new_position_y = position[1] - '1' + direction[i][1];
    if (new_position_x < 0 || new_position_x > 8) continue;
    if (new_position_y < 0 || new_position_y > 8) continue;
    if (mines[new_position_x][new_position_y] == '*') count++;
  }
  return count;
}

MoveResult make_move(const string position, char mines[9][9],
                     char revealed[9][9]) {
  // check legal position
  if (position.length() < 2 || position.length() > 3) return INVALID_MOVE;
  if (position.length() == 3 && position[2] != '*') return INVALID_MOVE;
  if (position[0] < 'A' || position[0] > 'I') return INVALID_MOVE;
  if (position[1] < '1' || position[1] > '9') return INVALID_MOVE;

  int new_position_x = position[0] - 'A';
  int new_position_y = position[1] - '1';
  if (revealed[new_position_x][new_position_y] != '?') return REDUNDANT_MOVE;

  if (position.length() == 2 && mines[new_position_x][new_position_y] == '*')
    return BLOWN_UP;

  if (position.length() == 3) {
    revealed[new_position_x][new_position_y] = '*';
    return VALID_MOVE;
  }

  if (is_complete(mines, revealed)) return SOLVED_BOARD;
  
  revealed[new_position_x][new_position_y] = ' ';
  if (count_mines(position, mines)) {
    revealed[new_position_x][new_position_y] =
        '0' + count_mines(position, mines);
    return VALID_MOVE;
  }

  string new_position = position;

  int direction[8][2] = {{1, 0},  {1, 1},   {0, 1},  {-1, 1},
                         {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

  for (int i = 0; i < 8; i++) {
    // new_position_x = position[0] - 'A' + direction[i][0];
    // new_position_y = position[1] - '1' + direction[i][1];
    new_position[0] = position[0] + direction[i][0];
    new_position[1] = position[1] + direction[i][1];
    if (make_move(new_position, mines, revealed) != VALID_MOVE) continue;
  }
  return VALID_MOVE;
}

bool find_safe_move(char revealed[9][9], char move[]) {
  char revealed_cpy[9][9];
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      revealed_cpy[i][j] = revealed[i][j];
    }
  }
  int direction[8][2] = {{1, 0},  {1, 1},   {0, 1},  {-1, 1},
                         {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
  int adj_x, adj_y, count_move = 0;
  string new_position = "A1";
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      if (revealed_cpy[y][x] != ' ' || revealed_cpy[y][x] != '?' || revealed_cpy[y][x] != '*') {
        int count = 0; // track unrevealed
        int count_mines = 0; // track *
        for (int i = 0; i < 8; i++) {
          adj_y = y + direction[i][0];
          adj_x = x + direction[i][1];
          if (adj_y < 0 || adj_y > 8) continue;
          if (adj_x < 0 || adj_x > 8) continue;
          if (revealed_cpy[adj_y][adj_x] == '?') count++;
          if (revealed_cpy[adj_y][adj_x] == '*') count_mines++;
        }
        if (revealed_cpy[y][x] - '0' == count + count_mines) {
          // all ? is *
          for (int i = 0; i < 8; i++) {
            adj_y = y + direction[i][0];
            adj_x = x + direction[i][1];
            if (adj_y < 0 || adj_y > 8) continue;
            if (adj_x < 0 || adj_x > 8) continue;
            if (revealed_cpy[adj_y][adj_x] == '?') {
              revealed_cpy[adj_y][adj_x] = '*';
              move[count_move++] = adj_y + 'A';
              move[count_move++] = adj_x + '1';
              move[count_move++] = '*';
              move[count_move] = '\0';
              return true;
              // display_board(revealed_cpy);
            }
          }
        } else if (revealed_cpy[y][x] - '0' == count_mines && count) {
          // all ? is safe
          for (int i = 0; i < 8; i++) {
            adj_y = y + direction[i][0];
            adj_x = x + direction[i][1];
            if (adj_y < 0 || adj_y > 8) continue;
            if (adj_x < 0 || adj_x > 8) continue;
            if (revealed_cpy[adj_y][adj_x] == '?') {
              revealed_cpy[adj_y][adj_x] = ' ';
              move[count_move++] = adj_y + 'A';
              move[count_move++] = adj_x + '1';
              move[count_move] = '\0';
              // display_board(revealed_cpy);
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

void solve_board(char mines[9][9], char revealed[9][9], char move[]) {
  char safe_move[512];
  while (true) {
    if (find_safe_move(revealed, safe_move)) {
      cout << safe_move << " ";
      make_move(safe_move, mines, revealed);
    } else {
      // make risky move
      break;
    }
  }
  display_board(revealed);
  display_board(mines);

}