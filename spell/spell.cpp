#include "spell.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/* insert your function definitions here */
int frequency(const char* target) {
  ifstream in;
  in.open("words.dat");

  if (!in) {
    cout << "Cannot open words.dat file" << endl;
    return -1;
  }

  int count;
  string word;
  while (!in.eof()) {
    in >> count >> word;

    if (strcmp(target, word.c_str()) == 0) {
      in.close();
      return count;
    }
  }
  in.close();
  return 0;
}

int edit_distance(const char* a, const char* b) {
  int length_a = strlen(a);
  int length_b = strlen(b);

  if (length_a == length_b) {
    if (strcmp(a, b) == 0) {
      return 0;
    }
  }

  return edit_distance(a, b, length_a, length_b);
}

int edit_distance(const char* a, const char* b, int length_a, int length_b) {
  int result, new_result;

  // if min(i,j) = 0; return max(i,j)
  if (length_a == 0) {
    return length_b;
  } else if (length_b == 0) {
    return length_a;
  }

  // if i,j > 1
  // if a(i-1)=b(j-2) and a(i-2)=b(j-1)
  if (length_a > 1 && length_b > 1 && a[length_a - 1] == b[length_b - 2] &&
      a[length_a - 2] == b[length_b - 1]) {
    result = edit_distance(a, b, length_a - 1, length_b) + 1;
    new_result = edit_distance(a, b, length_a, length_b - 1) + 1;
    if (new_result < result) {
      result = new_result;
    }
    new_result = edit_distance(a, b, length_a - 1, length_b - 1) +
                 (a[length_a - 1] == b[length_b - 1] ? 0 : 1);
    if (new_result < result) {
      result = new_result;
    }
    new_result = edit_distance(a, b, length_a - 2, length_b - 2) + 1;
    if (new_result < result) {
      result = new_result;
    }
    return result;
  } else {
    result = edit_distance(a, b, length_a - 1, length_b) + 1;
    new_result = edit_distance(a, b, length_a, length_b - 1) + 1;
    if (new_result < result) {
      result = new_result;
    }
    new_result = edit_distance(a, b, length_a - 1, length_b - 1) +
                 (a[length_a - 1] == b[length_b - 1] ? 0 : 1);
    if (new_result < result) {
      result = new_result;
    }

    return result;
  }
}

bool spell_correct(const char * word, char * fixed) {

  if (frequency(word)) {
    return false;
  }

  ifstream in;
  in.open("words.dat");

  int lowest_distance = 0, highest_freq;
  string best_suggestion;

  if (!in) {
    cerr << "Cannot open file words.dat" << endl;
    return false;
  }

  int count, distance;
  string temp_word;
  while (!in.eof()) {
    in >> count >> temp_word;

    if (strcmp(word, temp_word.c_str()) != 0) {

      distance = edit_distance(word, temp_word.c_str());
      // cout << temp_word << "\n";
      // if (lowest_distance == 1) break;
      if (lowest_distance == 0) {
        lowest_distance = distance;
        best_suggestion = temp_word;
        highest_freq = count;
      }
      if (distance < lowest_distance) {
        lowest_distance = distance;
        best_suggestion = temp_word;
        highest_freq = count;
      }
      if (distance == lowest_distance && count > highest_freq) {
        lowest_distance = distance;
        best_suggestion = temp_word;
        highest_freq = count;
      }
    }
  }
  in.close();
  cout << best_suggestion << endl;
  strcpy(fixed, best_suggestion.c_str());
  return true;
}