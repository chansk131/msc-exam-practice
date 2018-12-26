#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* new function */
bool find_marker(const char ch, char **maze, int height, int width, int &row, int &column) {
  for (int r=0; r<height; r++) {
    for (int c=0; c<width; c++) 
      if (maze[r][c] == ch) {
        row = r;
        column = c;
        return true;
      }
  }
  return false;
}

void get_direction(const char path, int& row, int& column) {
  switch (path) {
    case 'N': {
      row--;
      break;
    }
    case 'S': {
      row++;
      break;
    }
    case 'E': {
      column++;
      break;
    }
    case 'W': {
      column--;
      break;
    }
  }
}

bool valid_solution(const char * path, char **maze, int height, int width) {
  int exit_row, exit_column;
  int row = 0, col = 0;
  find_marker('>', maze, height, width, row, col);
  find_marker('X', maze, height, width, exit_row, exit_column);

  for (int i = 0; path[i]; i++) {
    get_direction(path[i], row, col);
    if (row < 0 || row >= height) return false;
    if (col < 0 || col >= height) return false;
    if (maze[row][col] == 'X') break;
    if (maze[row][col] != ' ') return false;
  }

  if (row == exit_row && col == exit_column) return true;
  return false;
}

bool get_valid_move(char **maze, int height, int width, int current_row, int current_column, const char end, int &next_row, int &next_col, int i = 0) {
  static char direction[4] = {'N', 'S', 'E', 'W'};
  for (; i < 4; i++) {
    next_row = current_row;
    next_col = current_column;
    get_direction(direction[i], next_row, next_col);
    if (next_row < 0 || next_row >= height) continue;
    if (next_col < 0 || next_col >= height) continue;
    if (maze[next_row][next_col] != ' ' && maze[next_row][next_col] != end)
      continue;
    return true;
  }
  return false;
}

bool find_legal_path(char **maze, int height, int width, const char end, int &current_row, int &current_col, int& count, char * path) {
  static char direction[4] = {'N', 'S', 'E', 'W'};
  int next_row, next_col, end_row, end_col;

  for (int i = 0; i < 4; i++) {
    if (!find_marker(end, maze, height, width, end_row, end_col)) {
      *path = '\0';
      return true;
    }
    next_row = current_row;
    next_col = current_col;
    get_direction(direction[i], next_row, next_col);
    if (next_row < 0 || next_row >= height) continue;
    if (next_col < 0 || next_col >= width) continue;
    if (maze[next_row][next_col] != ' ' && maze[next_row][next_col] != end) 
      continue;
    maze[next_row][next_col] = '#';
    *path = direction[i];
    if (find_legal_path(maze, height, width, end, next_row, next_col, count, path + 1)) {
      return true;
    }
    maze[next_row][next_col] = ' ';
  }
  return false;
}

char* find_path(char **maze, int height, int width, const char start, const char end) {
  static char path[512];
  int current_row, current_col, count;

  // start first one
  find_marker(start, maze, height, width, current_row, current_col);
  maze[current_row][current_col] = '#';
  // find path recursively
  if (!find_legal_path(maze, height, width, end, current_row, current_col, count, path)) {
    strcpy(path, "Unknown");
    maze[current_row][current_col] = start;
    return path;
  }
  return path;
}