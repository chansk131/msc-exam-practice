#include "playfair.h"

using namespace std;

void prepare(const string input, char * output) {
  string str_output;
  for (auto const& ch: input) {
    if (isalnum(ch))
      str_output += toupper(ch);
  }

  if (str_output.size() % 2 == 1)
    str_output += 'X';
  
  strcpy(output, str_output.c_str());
}

void grid(const string codeword, char square[6][6]) {
  string alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  string str_square;
  int count = 0;
  for (auto const& ch: codeword) {
    size_t found = alphanum.find(ch);
    if (found != string::npos) {
      square[count / 6][count % 6] = ch;
      count++;
      alphanum.erase(alphanum.begin() + found);
    }
  }

  for (auto const& ch: alphanum) {
    square[count / 6][count % 6] = ch;
    count++;
  }
}

void get_position(const char square[6][6], const char ch, int &row, int &col) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      if (square[i][j] == ch) {
        row = i;
        col = j;
        return;
      }
    }
  }
}

void bigram(const char square[6][6], const char inchar1, const char inchar2, char &outchar1, char &outchar2) {
  int row1, row2, col1, col2;

  get_position(square, inchar1, row1, col1);
  get_position(square, inchar2, row2, col2);

  outchar1 = square[row1][col2];
  outchar2 = square[row2][col1];
}

void encode(const char square[6][6], const char *prepared, char *encoded) {
  if (!*prepared) {
    encoded[0] = '\0';
    return;
  }

  bigram(square, prepared[0], prepared[1], encoded[0], encoded[1]);

  encode(square, prepared+2, encoded+2);
}

void decode(const char square[6][6], const char *encoded, char *decoded) {
  if (!*encoded) {
    decoded[0] = '\0';
    return;
  }

  bigram(square, encoded[0], encoded[1], decoded[0], decoded[1]);

  decode(square, encoded+2, decoded+2);
}