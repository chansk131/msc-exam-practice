#include "playfair.h"
#include <cctype>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

void prepare(const char* input, char* output) {
  string temp;

  for (int i = 0; *(input + i); i++) {
    if (isalnum(*(input + i))) {
      if (isalpha(*(input + i))) {
        temp += toupper(*(input + i));
      } else {
        temp += *(input + i);
      }
    }
  }

  if (temp.length() % 2) temp += 'X';

  strcpy(output, temp.c_str());
}

bool is_duplicated(char input, const char* reference) {
  string ref(reference);
  int ref_len = ref.length();
  for (int i = 0; i < ref_len; i++) {
    if (input == ref[i]) return true;
  }
  return false;
}

void grid(const char* codeword, char square[6][6]) {
  string alphanum("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

  string temp(codeword);

  int temp_len = temp.length();

  if (temp_len % 2) {
    temp_len--;
  }

  int temp_index = 0, str_len = temp_len;
  for (int i = 0; i < temp_len; i++) {
    for (int j = 0; j < i; j++) {
      if (temp[temp_index] == square[j % 6][j - (j % 6) * 6]) {
        temp_index++;
        str_len--;
        break;
      }
    }
    square[i % 6][i - (i % 6) * 6] = temp[temp_index];
    temp_index++;
  }

  int ref_index = 0;
  for (int i = str_len; i < 36; i++) {
    while (is_duplicated(alphanum[ref_index], codeword)) {
      ref_index++;
    }
    square[i%6][i - (i%6)*6] = alphanum[ref_index];
    ref_index++;
  }
}

void bigram(char square[6][6], const char inchar1, const char inchar2, char &outchar1, char &outchar2) {
  int row1 = -1, col1 = -1, row2 = -1, col2 = -1;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      if (square[i][j] == inchar1) {
        row1 = i;
        col1 = j;
      } else if (square[i][j] == inchar2) {
        row2 = i;
        col2 = j;
      } 
      // else if (row1 + col1 + row2 + col2 == -4) {
      //   break;
      // }

    }
  }

  outchar1 = square[row1][col2];
  outchar2 = square[row2][col1];
}

void encode(char square[6][6], const char * prepared, char * encoded) {
  if (!*prepared) {
    *encoded = '\0';
    return;
  }

  bigram(square, *prepared, *(prepared + 1), *encoded, *(encoded + 1));
  return encode(square, prepared + 2, encoded + 2);
}

void decode(char square[6][6], const char * encoded, char * decoded) {
  if (!*encoded) {
    *decoded = '\0';
    return;
  }
  bigram(square, *encoded, *(encoded + 1), *decoded, *(decoded + 1));
  return encode(square, encoded + 2, decoded + 2);
}