#include "playfair.h"
#include <cctype>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

void prepare(const char* input, char* output) {
  int count = 0;
  // convert store alpha numeric to upper in output
  for (int i = 0; input[i]; i++) {
    if (isalnum(input[i]))
      output[count++] = toupper(input[i]);
  }

  // add 'X' if odd length
  if (count % 2)
    output[count++] = 'X';

  // add sentinel
  output[count] = '\0';
}

bool occurs_before(const char str[], char letter, int pos) {
  for (int i = 0; i < pos; i++) {
    if (str[i] == letter) return true;
  }
  return false;
}

void grid(const char* codeword, char square[6][6]) {
  string alphanum("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
  string temp(codeword);
  int temp_len;

  if (temp.length() % 2) {
    temp.pop_back();
  }
  temp += alphanum;
  temp_len = temp.length();

  int temp_index = 0, str_len = temp_len;
  for (int i = 0; i < 36; i++) {
    while (occurs_before(temp.c_str(), temp[temp_index], temp_index)) {
      temp_index++;
      str_len--;
    }
    square[i % 6][i - (i % 6) * 6] = temp[temp_index];
    temp_index++;
  }
}

void bigram(char square[6][6], const char inchar1, const char inchar2, char &outchar1, char &outchar2) {
  bool isFinished = false;
  int row1 = -1, col1 = -1, row2 = -1, col2 = -1;
  for (int i = 0; i < 6 && !isFinished; i++) {
    for (int j = 0; j < 6 && !isFinished; j++) {
      if (square[i][j] == inchar1) {
        row1 = i;
        col1 = j;
      } else if (square[i][j] == inchar2) {
        row2 = i;
        col2 = j;
      } 
      else if (row1 >=0 && col1 >=0 && row2 >=0 && col2 >= 0) {
        isFinished = true;
      }

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