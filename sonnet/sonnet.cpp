#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <string>
#include <fstream>

using namespace std;

#include "sonnet.h"

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }
  
  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  // the table which maps endings to letters (persists between calls)
  static map<string, char> lookup;

  // providing a way to reset the table between poems
  if (ending == RESET) {
    lookup.clear();
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  return lookup[end];
}

/* START WRITING YOUR FUNCTION BODIES HERE */
int count_words(const char *input_line) {
  int count = 0;
  if (!*input_line) return count;
  char word[512];
  count++;
  while (get_word(input_line, count, word)) {
    count++;
  }
  return (count - 1);
}

int get_last_vowel(const char * word) {
  char phonetic[512];
  int position = 1, last_vowel_position = -1;
  while (get_word(word, position, phonetic)) {
    switch (phonetic[0]) {
      case 'A':
      case 'E':
      case 'I':
      case 'O':
      case 'U':
        last_vowel_position = position;
    }
    position++;
  }
  return last_vowel_position;
}

void get_ending(const char * word, char * phonetic_ending, int position) {
  string temp;
  char phonetic[512];
  while (get_word(word, position, phonetic)) {
    temp += phonetic;
    position++;
  }
  strcpy(phonetic_ending, temp.c_str());
}

bool find_phonetic_ending(const char * word, char * phonetic_ending) {
  bool success = false;
  ifstream in;
  in.open("dictionary.txt");
  if (in.fail()) {
    cerr << "Cannot open dictionary.txt file";
    in.close();
    return success;
  }

  string line;
  char first[512];
  int first_vowel_position;
  while (!in.eof()) {
    getline(in, line);
    get_word(line.c_str(), 1, first);
    if (!strcmp(word, first)) {
      // get first vowel
      first_vowel_position = get_last_vowel(line.c_str());
      get_ending(line.c_str(), phonetic_ending, first_vowel_position);
      success = true;
      break;
    }
  }
  in.close();
  return success;
}

int get_last_word_position(const char * word) {
  int position = 1;
  char last_word[512];
  while (get_word(word, position, last_word)) {
    position++;
  }
  return position - 1;
}

bool find_rhyme_scheme(const char * filename, char * scheme) {
  bool success = false;
  ifstream in;
  in.open(filename);
  if (in.fail()) {
    cerr << "Cannot open " << filename << " file";
    in.close();
    return success;
  }

  rhyming_letter(RESET);
  string line;
  char word[512], phonetics[512];
  int word_count = 1, scheme_count = 0, last_word_position;
  while (!in.eof()) {
    getline(in, line);
    // cout << line << endl;
    word_count = 1;
    last_word_position = get_last_word_position(line.c_str());
    get_word(line.c_str(), last_word_position, word);
    if (find_phonetic_ending(word, phonetics)) {
      // cout << phonetics << endl;
      scheme[scheme_count++] = rhyming_letter(phonetics);
    }
  }
  scheme[scheme_count] = '\0';
  in.close();
  return true;
}

string identify_sonnet(const char * filename) {

  char sonnet[512], reference[512];

  find_rhyme_scheme(filename, sonnet);
  find_rhyme_scheme("shakespeare.txt", reference);
  if (!strcmp(sonnet, reference)) return "Shakespearean";

  find_rhyme_scheme("petrarch.txt", reference);
  if (!strcmp(sonnet, reference)) return "Petrarchan";

  find_rhyme_scheme("spenser.txt", reference);
  if (!strcmp(sonnet, reference)) return "Spenserian";

  return "Unknown";
}