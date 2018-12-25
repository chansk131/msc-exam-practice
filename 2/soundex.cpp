#include "soundex.h"
#include <cctype>
#include <cstring>
#include <iostream>

using namespace std;

void encode(const char* surname, char soundex[]) {
  soundex[0] = *surname;

  int count = 1, index = 1;
  while (*(surname + count) && index < 4) {
    switch (*(surname + count)) {
      case 'b':
      case 'f':
      case 'p':
      case 'v':
        soundex[index] = '1';
        break;
      case 'c':
      case 'g':
      case 'j':
      case 'k':
      case 'q':
      case 's':
      case 'x':
      case 'z':
        soundex[index] = '2';
        break;
      case 'd':
      case 't':
        soundex[index] = '3';
        break;
      case 'l':
        soundex[index] = '4';
        break;
      case 'm':
      case 'n':
        soundex[index] = '5';
        break;
      case 'r':
        soundex[index] = '6';
        break;
      default:
        index--;
        break;
    }
    if (soundex[index] == soundex[index - 1]) index--;
    count++;
    index++;
  }
  while (index < 4)
  {
    soundex[index] = '0';
    index++;
  }
  soundex[index] = '\0';
}

int compare(const char * one, const char * two)
{
  if (!*one) return 1;
  if (!*two) return 0;

  if (*one == *two) {
    return compare(one + 1, two + 1);
  }
  
  return 0;
}

int count(const char * surname, const char * sentence)
{
  char soundex[5];
  encode(surname, soundex);

  int start = 0, end = 0, count = 0;
  char sentence_surname[20];
  char sentence_soundex[5];
  while (*(sentence + end))
  {
    if (isalpha(*(sentence + end))) {
      sentence_surname[start] = *(sentence + end);
      start++;
    } else {
      sentence_surname[start] = '\0';
      start = 0;
      encode(sentence_surname, sentence_soundex);
      if (!strcmp(sentence_soundex, soundex))
        count++;
    }
    end++;
  }
  return count;
}