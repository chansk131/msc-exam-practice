#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r = 0; r < rows; r++) {
    m[r] = new (nothrow) char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r = 0; r < rows; r++) delete[] m[r];
  delete[] m;
}

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i = 0; i < SCENE_HEIGHT; i++)
    for (int j = 0; j < SCENE_WIDTH; j++) scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene
 */
void destroy_scene(char **scene) { deallocate_2D_array(scene, SCENE_HEIGHT); }

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i = 0; i < SCENE_HEIGHT; i++) {
    for (int j = 0; j < SCENE_WIDTH; j++) cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {
  ifstream in(filename);
  if (!in) return false;

  int start_col = col;
  char line[512];
  in.getline(line, 512);
  filter(line);
  while (!in.fail()) {
    for (int n = 0; n < strlen(line); n++) {
      if (row >= SCENE_HEIGHT) return false;
      if (col >= SCENE_WIDTH) break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line, 512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in
 * Question 3 */
const char *status_description(int code) {
  switch (code) {
    case ERROR_INVALID_CANNIBAL_COUNT:
      return "Invalid cannibal count";
    case ERROR_INVALID_MISSIONARY_COUNT:
      return "Invalid missionary count";
    case ERROR_INVALID_MOVE:
      return "Invalid move";
    case ERROR_MISSIONARIES_EATEN:
      return "The missionaries have been eaten! Oh dear!";
    case ERROR_INPUT_STREAM_ERROR:
      return "Unexpected input stream error";
    case ERROR_BONUS_NO_SOLUTION:
      return "No solution";
    case VALID_GOAL_STATE:
      return "Problem solved!";
    case VALID_NONGOAL_STATE:
      return "Valid state, everything is OK!";
  }
  return "Unknown error";
}

struct Count {
  int missionary;
  int cannibal;
  bool is_equal(const Count right) {
    if (cannibal == right.cannibal && missionary == right.missionary) {
      return true;
    } else {
      return false;
    }
  }
};

bool get_left_count(const char *left, Count &left_count, int &boat_position) {
  string input(left);
  int count = input.length();

  if (count > 7) {
    cerr << "length of left is more than 7 char" << endl;
    return false;
  }

  left_count.cannibal = 0;
  left_count.missionary = 0;

  for (int i = 0; i < count; i++) {
    if (input[i] != 'M' && input[i] != 'C' && input[i] != 'B') {
      cerr << "wrong input left" << endl;
      return false;
    } else if (input[i] == 'M') {
      left_count.missionary++;
    } else if (input[i] == 'C') {
      left_count.cannibal++;
    } else if (input[i] == 'B') {
      boat_position = 1;
    }
  }
  return true;
}

bool get_boat_count(const char *boat, Count &boat_count) {
  string input(boat);
  int count = input.length();

  if (count > 2) {
    cerr << "length of left is more than 7 char" << endl;
    return false;
  }

  boat_count.cannibal = 0;
  boat_count.missionary = 0;

  for (int i = 0; i < count; i++) {
    if (input[i] != 'M' && input[i] != 'C') {
      cerr << "wrong input left" << endl;
      return false;
    } else if (input[i] == 'M') {
      boat_count.missionary++;
    } else if (input[i] == 'C') {
      boat_count.cannibal++;
    }
  }
  return true;
}

/* insert your functions here */

char **make_river_scene(const char *left, const char *boat) {
  char **scene = create_scene();
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
  add_to_scene(scene, 19, 19, "river.txt");

  Count left_count, boat_count, right_count;
  int boat_position = 0;

  // check left input
  if (!get_left_count(left, left_count, boat_position)) return scene;

  // check boat input
  if (!get_boat_count(boat, boat_count)) return scene;

  right_count.missionary = 3 - left_count.missionary - boat_count.missionary;
  right_count.cannibal = 3 - left_count.cannibal - boat_count.cannibal;

  // draw left bank
  for (int i = 0; i < left_count.missionary; i++) {
    add_to_scene(scene, 2, 6 * i + 1, "missionary.txt");
  }
  for (int i = 0; i < left_count.cannibal; i++) {
    add_to_scene(scene, 11, 6 * i + 1, "cannibal.txt");
  }

  int boat_draw_position;
  if (boat_position) {
    boat_draw_position = 19;
  } else {
    boat_draw_position = 36;
  }
  // draw boat
  add_to_scene(scene, 17, boat_draw_position, "boat.txt");
  for (int i = 0; i < boat_count.missionary; i++) {
    add_to_scene(scene, 11, 6 * i + boat_draw_position + 3, "missionary.txt");
  }
  for (int i = 0; i < boat_count.cannibal; i++) {
    add_to_scene(scene, 11, 6 * i + boat_draw_position + 3, "cannibal.txt");
  }

  // draw right bank
  for (int i = 0; i < right_count.missionary; i++) {
    add_to_scene(scene, 2, 6 * i + 54, "missionary.txt");
  }
  for (int i = 0; i < right_count.cannibal; i++) {
    add_to_scene(scene, 11, 6 * i + 54, "cannibal.txt");
  }

  return scene;
}

Status perform_crossing(char *left, const char *targets) {
  Status state = VALID_GOAL_STATE;

  Count left_count, boat_count, right_count;
  int boat_position = 0;

  // check left input
  if (!get_left_count(left, left_count, boat_position)) {
  }

  // check boat input
  if (!get_boat_count(targets, boat_count)) {
  }

  if (boat_position) {
    left_count.missionary -= boat_count.missionary;
    left_count.cannibal -= boat_count.cannibal;
  }
  right_count.missionary = 3 - left_count.missionary - boat_count.missionary;
  right_count.cannibal = 3 - left_count.cannibal - boat_count.cannibal;

  if (right_count.missionary < 0) {
    return ERROR_INVALID_MOVE;
  }
  if (right_count.cannibal < 0) {
    return ERROR_INVALID_MOVE;
  }

  // loading the boat
  if (boat_position) {
    string new_left = "B";
    for (int i = 0; i < left_count.missionary; i++) {
      new_left += "M";
    }
    for (int i = 0; i < left_count.cannibal; i++) {
      new_left += "C";
    }
    strcpy(left, new_left.c_str());
  }
  char **scene = create_scene();
  scene = make_river_scene(left, targets);
  // print_scene(scene);

  // crossing the river
  string new_left;
  if (!boat_position) {
    boat_position = 1;
    new_left += "B";
  } else {
    boat_position = 0;
  }
  for (int i = 0; i < left_count.missionary; i++) {
    new_left += "M";
  }
  for (int i = 0; i < left_count.cannibal; i++) {
    new_left += "C";
  }
  strcpy(left, new_left.c_str());
  scene = make_river_scene(left, targets);
  // print_scene(scene);

  // unloading boat
  if (boat_position) {
    new_left = "B";
    left_count.missionary += boat_count.missionary;
    left_count.cannibal += boat_count.cannibal;
    for (int i = 0; i < left_count.missionary; i++) {
      new_left += "M";
    }
    for (int i = 0; i < left_count.cannibal; i++) {
      new_left += "C";
    }
    strcpy(left, new_left.c_str());
  } else {
    right_count.missionary += boat_count.missionary;
    right_count.cannibal += boat_count.cannibal;
  }
  boat_count.cannibal = 0;
  boat_count.missionary = 0;

  scene = make_river_scene(left, "");
  // print_scene(scene);

  // check result
  if (right_count.missionary && right_count.missionary < right_count.cannibal) {
    state = ERROR_MISSIONARIES_EATEN;
  } else if (new_left.length() == 0) {
    state = VALID_GOAL_STATE;
  } else {
    state = VALID_NONGOAL_STATE;
  }

  return state;
}

Status play_game() {
  string target;

  char left[8] = "BCCCMMM";

  Status state = VALID_NONGOAL_STATE;

  while (state == VALID_NONGOAL_STATE) {
    cin >> target;
    state = perform_crossing(left, target.c_str());
  }

  return state;
}

struct Move {
  Count count;
  string move;
};

bool is_history_duplicated(vector<Move> &history, Move &move) {
  if (!history.size()) return false;
  vector<Move>::iterator it;
  for(it = history.begin(); it != history.end(); ++it) {
    if ((*it).move == move.move) {
      if ((*it).count.is_equal(move.count)) {
        return true;
      }
    }
  }
  return false;
}

string moves[] = {"MM", "MC", "CC", "M", "C"};

Status find_solution(char *left, char *answer) {
  // string save_left(left);
  // Status state;
  // Move move;
  // Count left_count;
  // int boat_position;

  // vector<Move> history;

  // if (strcmp(left, "") == 0) {
  //   return VALID_GOAL_STATE;
  // }

  // while (true) {
  //   for (int i = 0; i < 5; i++) {

  //     state = perform_crossing(left, moves[i].c_str());

  //     if (state != VALID_GOAL_STATE && state != VALID_NONGOAL_STATE) {
  //       strcpy(left, save_left.c_str());
  //     } else if (state == VALID_NONGOAL_STATE) {
  //       get_left_count(left, left_count, boat_position);
  //       // check for duplicate
  //       move.count = left_count;
  //       move.move = moves[i];
  //       if (is_history_duplicated(history, move)) {
  //         strcpy(left, save_left.c_str());
  //         // cout << "reverted " << left << " ";
  //       } else {
  //         history.push_back(move);
  //         save_left = left;
  //         cout << move.move << "," << left << endl;
  //       }

  //     } else {

  //       return VALID_GOAL_STATE;
  //     }

  //   }
  //   if (!history.size()) {
  //     return ERROR_BONUS_NO_SOLUTION;
  //   } else {
  //     cout << "new " << left << " " << save_left << endl;
  //     strcpy(left, save_left.c_str());
  //   }
  // }

  
  // return state;
}