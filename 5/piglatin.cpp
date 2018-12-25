#include <cctype>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include "piglatin.h"

using namespace std;


bool isVowel(char ch, int position, int length) {
  switch (ch) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
      return true;
    case 'y': {
      if (position == 0 || position == length - 1) return false;
      else return true;
    }
    default:
      return false;
  }
}

int findFirstVowel(const char * text) {
  string str_text(text);
  int str_len = str_text.length();
  for (int i = 0; i < str_len; i++) {
    if (!isalpha(*(text + i))) continue;
    if (isVowel(*(text + i), i, str_len))
      return i;
  }
  return -1;
}

void translateWord(const char * text, char piglatin[]) {
  string temp_piglatin(text);

  if (!*text) return;

  // begins with vowel, add "way"
  int vowel_position = findFirstVowel(text);
  if (vowel_position == 0) {
    temp_piglatin += "way";
  }
  else if (isdigit(*text)) {
    // temp = text
  }
  // no vowel
  else if (vowel_position == -1) {
    // temp = text + ay
    temp_piglatin += "ay";
  }
  // begin with a letter not vowel, start with vowel, add remaining and "ay"
  else if (isalpha(*text)) {
    temp_piglatin = text + vowel_position;
    for (int i = 0; i < vowel_position; i++) {
      temp_piglatin += tolower(*(text + i));
    }
    temp_piglatin += "ay";
    if (isupper(*text)) {
      temp_piglatin[0] = toupper(temp_piglatin[0]);
    }
  }
  strcpy(piglatin, temp_piglatin.c_str());
}

void get_word(ifstream& input, char text[], char& punc) {
  string temp_text;

  char ch;

  while (true) {
    input.get(ch);
    if (isalpha(ch) || isdigit(ch)) {
      temp_text += ch;
    } else {
      punc = ch;
      break;
    }
  }
  strcpy(text, temp_text.c_str());
}

void translateStream(ifstream& input, ostream& cout) {
  if (input.fail()) return;

  char text[512], piglatin[512], punc;
  get_word(input, text, punc);
  translateWord(text, piglatin);
  cout << piglatin << punc;
  translateStream(input, cout);
}
