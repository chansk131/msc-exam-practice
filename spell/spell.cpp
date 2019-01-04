#include "spell.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

  int cache[MAX_LENGTH][MAX_LENGTH] = {{0}};

  return edit_distance(a, b, length_a, length_b, cache);
}

int edit_distance(const char* a, const char* b, int i, int j,
                  int cache[MAX_LENGTH][MAX_LENGTH]) {
  if (min(i, j) == 0) {
    return max(i, j);
  }

  if (cache[i][j]) return cache[i][j];

  vector<int> result;
  result.push_back(edit_distance(a, b, i - 1, j, cache) + 1);
  result.push_back(edit_distance(a, b, i, j - 1, cache) + 1);
  result.push_back(edit_distance(a, b, i - 1, j - 1, cache) +
                   (a[i - 1] == b[j - 1] ? 0 : 1));
  if (i > 1 && j > 1 && a[i - 1] == b[j - 2] &&
      a[i - 2] == b[j - 1]) {
    result.push_back(edit_distance(a, b, i - 2, j - 2, cache) +
                     1);
  }

  return cache[i][j] = *min_element(result.begin(), result.end());
}

bool spell_correct(const char* word, char* fixed) {
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
      if (lowest_distance == 0 || distance < lowest_distance ||
          (distance == lowest_distance && count > highest_freq)) {
        lowest_distance = distance;
        best_suggestion = temp_word;
        highest_freq = count;
      }
    }
  }
  in.close();
  strcpy(fixed, best_suggestion.c_str());
  return true;
}