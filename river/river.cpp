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

/* insert your functions here */
bool get_left_count(const string left, int &left_missionary, int &left_cannibal, int &boat_position) {
  if (left.size() > 7) {
    return false;
  }
  
  boat_position = count(left.begin(), left.end(), 'B');
  left_missionary = count(left.begin(), left.end(), 'M');
  left_cannibal = count(left.begin(), left.end(), 'C');

  if (boat_position > 1 || left.size() != boat_position + left_missionary + left_cannibal) {
    return false;
  }

  return true;
}

bool get_boat_count(const string boat, int &boat_missionary, int &boat_cannibal) {
  if (boat.size() > 2) {
    return false;
  }
  boat_missionary = count(boat.begin(), boat.end(), 'M');
  boat_cannibal = count(boat.begin(), boat.end(), 'C');

  if (boat.size() != boat_missionary + boat_cannibal) {
    return false;
  }

  return true;
}

char **make_river_scene(const string left, const string boat) {
  char **scene = create_scene();
  
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
  add_to_scene(scene, 19, 19, "river.txt");
  
  int left_missionary, left_cannibal;
  int boat_missionary, boat_cannibal;
  int right_missionary, right_cannibal;

  int boat_position;

  if (!get_left_count(left, left_missionary, left_cannibal, boat_position)) {
    return scene;
  }
  if (!get_boat_count(boat, boat_missionary, boat_cannibal)) {
    return scene;
  }

  right_missionary = 3 - left_missionary - boat_missionary;
  right_cannibal = 3 - boat_cannibal - left_cannibal;

  if (right_missionary < 0 || right_cannibal < 0 || right_missionary > 3 || right_cannibal > 3) {
    return scene;
  }

  // draw left bank
  for (int i = 0; i < left_missionary; i++) {
    add_to_scene(scene, 2, 6 * i + 1, "missionary.txt");
  }
  for (int i = 0; i < left_cannibal; i++) {
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
  for (int i = 0; i < boat_missionary; i++) {
    add_to_scene(scene, 11, 6 * i + boat_draw_position + 3, "missionary.txt");
  }
  for (int i = 0; i < boat_cannibal; i++) {
    add_to_scene(scene, 11, 6 * i + boat_draw_position + 3, "cannibal.txt");
  }

  // draw right bank
  for (int i = 0; i < right_missionary; i++) {
    add_to_scene(scene, 2, 6 * i + 54, "missionary.txt");
  }
  for (int i = 0; i < right_cannibal; i++) {
    add_to_scene(scene, 11, 6 * i + 54, "cannibal.txt");
  }

  return scene;
}

void make_and_print_scene(const string left, const string targets) {
  char **scene = make_river_scene(left, targets);
  print_scene(scene);
  destroy_scene(scene);
}

string generate_left(int missionary, int cannibal, int boat_position) {
  string left;
  for (int i = 0; i < missionary; i++) {
    left += 'M';
  }
  for (int i = 0; i < cannibal; i++) {
    left += 'C';
  }
  if (boat_position) {
    left += 'B';
  }
  return left;
}

Status perform_crossing(char *left, const string targets) {
  string temp_left;
  // check if targets can fit the boat
  if (targets.size() < 1 || targets.size() > 2) {
    return ERROR_INVALID_MOVE;
  }

  if (temp_left.size() > 7) {
    return ERROR_INPUT_STREAM_ERROR;
  }

  int left_missionary, left_cannibal;
  int boat_missionary, boat_cannibal;
  int right_missionary, right_cannibal;

  int boat_position;

  if (!get_left_count(left, left_missionary, left_cannibal, boat_position)) {
    return ERROR_INPUT_STREAM_ERROR;
  }
  if (!get_boat_count(targets, boat_missionary, boat_cannibal)) {
    return ERROR_INVALID_MOVE;
  }

  right_missionary = 3 - left_missionary;
  right_cannibal = 3 - left_cannibal;

  // loading boat
  if (boat_position) {
    left_missionary -= boat_missionary;
    left_cannibal -= boat_cannibal;
  } else {
    right_missionary -= boat_missionary;
    right_cannibal -= boat_cannibal;
  }

  if (right_missionary < 0 || left_missionary < 0) {
    return ERROR_INVALID_MISSIONARY_COUNT;
  } else if (right_cannibal < 0 || right_missionary < 0) {
    return ERROR_INVALID_CANNIBAL_COUNT;
  }

  string crossing_left;
  crossing_left = generate_left(left_missionary, left_cannibal, boat_position);

  // make_and_print_scene(crossing_left, targets);

  // crossing, change boat position
  if (boat_position) {
    crossing_left.pop_back();
    boat_position = 0;
  } else {
    crossing_left += 'B';
    boat_position = 1;
  }

  // make_and_print_scene(crossing_left, targets);

  // unload
  if (boat_position) {
    // boat at the left
    left_missionary += boat_missionary;
    left_cannibal += boat_cannibal;
  } else {
    right_missionary += boat_missionary;
    right_cannibal += boat_cannibal;
  }

  crossing_left = generate_left(left_missionary, left_cannibal, boat_position);

  // make_and_print_scene(crossing_left, "");

  strcpy(left, crossing_left.c_str());

  if ((left_missionary > 0 && left_missionary < left_cannibal) || (right_missionary > 0 && right_missionary < right_cannibal)) {
    return ERROR_MISSIONARIES_EATEN;
  }

  if (crossing_left == "") {
    return VALID_GOAL_STATE;
  } else {
    return VALID_NONGOAL_STATE;
  }
}

Status play_game() {
  char left[] = "MMMCCCB";
  string target;
  Status state = VALID_NONGOAL_STATE;

  while (state == VALID_NONGOAL_STATE) {
    cout << "Please enter the target Missionary (M), cannibal (C)" << endl;
    cin >> target;

    state = perform_crossing(left, target);
  } 
  return state;
}

string targets[] = {"MM", "MC", "CC", "M", "C"};

struct History {
  string left, boat;
};

bool is_left_equal(string left1, char * left2) {
  int missionary1, cannibal1, boat_position1;
  int missionary2, cannibal2, boat_position2;

  get_left_count(left1, missionary1, cannibal1, boat_position1);
  get_left_count(left2, missionary2, cannibal2, boat_position2);

  if (missionary1 == missionary2 && cannibal1 == cannibal2 && boat_position1 == boat_position2) {
    return true;
  }

  return false;
}

bool is_state_equal(History history, char * left, string target) {
  if (history.boat != target) return false;
  return is_left_equal(history.left, left);
}

bool is_target_state_duplicated(char *left, string target, vector<History> &history) {

  if (history.size() % 2 && history.back().boat == target) return true;

  for(std::vector<History>::iterator it = history.begin(); it != history.end(); ++it) {
    if (is_state_equal(*it, left, target)) return true;
  }
  return false;
}

bool is_crossing_valid(char *left, string target) {
  Status state = perform_crossing(left, target);
  if (state == VALID_GOAL_STATE || state == VALID_NONGOAL_STATE) return true;
  return false;
}

Status find_solution(char *left, char *answer, vector<History> &history) {
  History now;
  now.left = left;

  Status state;
  for (int i = 0; i < 5; i++) {

    if (is_target_state_duplicated(left, targets[i], history)) {
      continue;
    }
    if (!is_crossing_valid(left, targets[i])) {
      strcpy(left, now.left.c_str());
      continue;
    }

    now.boat = targets[i];
    history.push_back(now);

    if (strcmp(left, "") == 0) {
      return VALID_GOAL_STATE;
    }

    state = find_solution(left, answer, history);
    if (state == VALID_GOAL_STATE) {
      return state;
    }
    strcpy(left, history.back().left.c_str());
    history.pop_back();
    
  }

  return ERROR_BONUS_NO_SOLUTION;
}

void get_answer(vector<History> &history, char *answer) {
  string temp_ans;
  for(std::vector<History>::iterator it = history.begin(); it != history.end(); ++it) {
    temp_ans += it->boat;
    temp_ans += ',';
  }

  temp_ans.pop_back();
  strcpy(answer, temp_ans.c_str());
}

Status find_solution(char *left, char *answer) {
  char save_left[8];
  strcpy(save_left, left);
  string target;
  Status state;

  vector<History> history;

  state = find_solution(left, answer, history);

  if (state != VALID_GOAL_STATE) {
    return ERROR_BONUS_NO_SOLUTION;
  }

  get_answer(history, answer);

  return state;
}