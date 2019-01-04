#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r = 0; r < rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r = 0; r < rows; r++) delete[] m[r];
  delete[] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
  char line[512];

  ifstream input(filename);

  height = width = 0;

  input.getline(line, 512);
  while (input) {
    if ((int)strlen(line) > width) width = strlen(line);
    height++;
    input.getline(line, 512);
  }

  if (height > 0) return true;
  return false;
}

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {
  bool success = get_map_dimensions(filename, height, width);

  if (!success) return NULL;

  char **m = allocate_2D_array(height, width);

  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r < height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ((int)strlen(m[r]) < width) strcat(m[r], space);
  }

  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " "
       << " ";
  for (int c = 0; c < width; c++)
    if (c && (c % 10) == 0)
      cout << c / 10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " "
       << " ";
  for (int c = 0; c < width; c++) cout << (c % 10);
  cout << endl;

  for (int r = 0; r < height; r++) {
    cout << setw(2) << r << " ";
    for (int c = 0; c < width; c++) cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3
 */
const char *error_description(int code) {
  switch (code) {
    case ERROR_START_STATION_INVALID:
      return "Start station invalid";
    case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
      return "Route endpoint is not a station";
    case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
      return "Line hopping between stations not possible";
    case ERROR_BACKTRACKING_BETWEEN_STATIONS:
      return "Backtracking along line between stations not possible";
    case ERROR_INVALID_DIRECTION:
      return "Invalid direction";
    case ERROR_OFF_TRACK:
      return "Route goes off track";
    case ERROR_OUT_OF_BOUNDS:
      return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n = 0; n < 8; n++) {
    if (!strcmp(token, strings[n])) return (Direction)n;
  }
  return INVALID_DIRECTION;
}

bool get_symbol_position(char **map, int height, int width, const char target,
                         int &r, int &c) {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      if (map[row][col] == target) {
        r = row;
        c = col;
        return true;
      }
    }
  }
  r = -1;
  c = -1;
  return false;
}

char get_symbol(const char *name, const char *filename) {
  string line;
  char symbol;
  ifstream in;
  in.open(filename);

  if (!in) {
    cerr << "Cannot open file line.txt" << endl;
    return '_';
  }

  while (!in.eof()) {
    getline(in, line);
    symbol = line[0];
    line.erase(0, 2);
    if (strcmp(line.c_str(), name) == 0) {
      in.close();
      return symbol;
    }
  }

  in.close();
  return '_';
}

char get_symbol_for_station_or_line(const char *name) {
  char symbol;
  symbol = get_symbol(name, "line.txt");

  if (symbol != '_') return symbol;

  return get_symbol(name, "stations.txt");
}

bool is_route_valid(char *route, vector<Direction> &directions) {
  string direction;
  Direction result;

  string temp_route(route);
  temp_route += ",";

  for (int i = 0; temp_route[i]; i++) {
    if (temp_route[i] != ',') {
      direction += temp_route[i];
    } else {
      result = string_to_direction(direction.c_str());
      direction.clear();
      if (result == INVALID_DIRECTION) {
        directions.clear();
        return false;
      } else {
        directions.push_back(result);
      }
    }
  }

  return true;
}

bool get_new_coordinate(int height, int width, Direction direction, int r,
                        int c, int &new_r, int &new_c) {
  if (direction == INVALID_DIRECTION) return false;
  new_r = r;
  new_c = c;
  switch(direction) {
    case N:
    case NW:
    case NE: {
      new_r--;
      break;
    }
    case S:
    case SW:
    case SE: {
      new_r++;
      break;
    }
    default: break;
  }

  switch (direction) {
    case E:
    case NE:
    case SE: {
      new_c++;
      break;
    }
    case W:
    case NW:
    case SW: {
      new_c--;
      break;
    }
    default: break;
  }

  if (new_r > height || new_r < 0 || new_c > width || new_c < 0) {
    new_r = r;
    new_c = c;
    return false;
  }

  return true;
}

bool is_at_station(const char symbol) {
  return isalnum(symbol);
}

string get_station_name(const char symbol) {
  string line;
  // search for line
  ifstream in;
  in.open("stations.txt");

  if (!in) {
    cerr << "Cannot open file line.txt" << endl;
    line = "";
    return line;
  }

  while (!in.eof()) {
    getline(in, line);
    if (line[0] == symbol) {
      in.close();
      line.erase(0, 2);
      return line;
    }
  }

  in.close();
  line = "";
  return line;
}

bool is_opposite_direction(Direction first, Direction second) {
  if (first == N && second == S) return true;
  if (first == S && second == N) return true;
  if (first == W && second == E) return true;
  if (first == E && second == W) return true;
  if (first == NE && second == SW) return true;
  if (first == SW && second == NE) return true;
  if (first == NW && second == SE) return true;
  if (first == SE && second == NW) return true;

  return false;
}

bool is_backtrack(vector<Direction> &directions) {
  int size = directions.size();
  for (int i = 1; i < size; i++) {
    if (is_opposite_direction(directions[i], directions[i-1])) {
      return true;
    }
  }
  return false;
}

int validate_route(char **map, int height, int width, const char *start_station,
                   char *route, char *destination) {

  // check if start_station is legal or not
  char station_symbol = get_symbol(start_station, "stations.txt");
  if (station_symbol == '_') {
    return ERROR_START_STATION_INVALID;
  }

  // check if route is legal or not then store in directions
  vector<Direction> directions;
  if (!is_route_valid(route, directions)) {
    return ERROR_INVALID_DIRECTION;
  }

  // check if rout is back tracking or not
  if (is_backtrack(directions)) {
    return ERROR_BACKTRACKING_BETWEEN_STATIONS;
  }

  // get position of starting_station
  int r, c;
  get_symbol_position(map, height, width, station_symbol, r, c);

  int new_r, new_c, change_count = 0;
  int direction_size = directions.size();

  char current_symbol, prev_symbol;
  char before_station = ' ';

  for (int i = 0; i < direction_size; i++) {
    prev_symbol = map[r][c];

    if (!get_new_coordinate(height, width, directions[i], r, c, new_r, new_c)) {
      return ERROR_OUT_OF_BOUNDS;
    }

    current_symbol = map[new_r][new_c];

    // update position
    r = new_r;
    c = new_c;

    if (current_symbol == ' ') {
      return ERROR_OFF_TRACK;
    }
    
    // if current_symbol differs from prev_symbol
    if (current_symbol != prev_symbol) {

      // now it is at the station
      if (is_at_station(current_symbol)) {
        // save symbol before it reaches station
        before_station = prev_symbol;
      }
      
      // now it passed the station by 1 step
      else if (is_at_station(prev_symbol)) {
        // if current_symbol differs from before_station, change line at station
        if (before_station != current_symbol) {
          change_count++;
        }

        // else, just passing the station without changing train
      } 
      
      // line hopping
      else {
        return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
      }
    }
  }

  if (!is_at_station(current_symbol)) {
    return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
  }

  string str_destination = get_station_name(current_symbol);

  strcpy(destination, str_destination.c_str());

  return change_count - 1;
}