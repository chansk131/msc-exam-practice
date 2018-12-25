#include <string>
#include <cctype>
#include <map>
#include <iostream>
#include <cstring>

using namespace std;

int encode_character(const char ch, char braille[]) {
  string temp_braille;
  map<char, string> braille_map;

  braille_map['.'] = ".0..00";
  braille_map[','] = ".0....";
  braille_map[';'] = ".00...";
  braille_map['-'] = "..0..0";
  braille_map['!'] = ".00.0.";
  braille_map['?'] = ".00..0";
  braille_map['('] = ".00.00";
  braille_map[')'] = ".00.00";
  braille_map[' '] = "......";
  braille_map['a'] = "0.....";
  braille_map['b'] = "00....";
  braille_map['c'] = "0..0..";
  braille_map['d'] = "0..00.";
  braille_map['e'] = "0...0.";
  braille_map['f'] = "00.0..";
  braille_map['g'] = "00.00.";
  braille_map['h'] = "00..0.";
  braille_map['i'] = ".0.0..";
  braille_map['j'] = ".0.00.";
  braille_map['k'] = "0.0...";
  braille_map['l'] = "000...";
  braille_map['m'] = "0.00..";
  braille_map['n'] = "0.000.";
  braille_map['o'] = "0.0.0.";
  braille_map['p'] = "0000..";
  braille_map['q'] = "00000.";
  braille_map['r'] = "000.0.";
  braille_map['s'] = ".000..";
  braille_map['t'] = ".0000.";
  braille_map['u'] = "0.0..0";
  braille_map['v'] = "000..0";
  braille_map['w'] = ".0.000";
  braille_map['x'] = "0.00.0";
  braille_map['y'] = "0.0000";
  braille_map['z'] = "0.0.00";

  int num = 0;
  // check for punc
  if (ispunct(ch)) {
    num = 6;
    temp_braille = braille_map[ch];
  } else if (isalpha(ch)) {
    if (isupper(ch)) {
      num = 12;
      temp_braille = ".....0";
      temp_braille += braille_map[tolower(ch)];
    } else {
      num = 6;
      temp_braille = braille_map[ch];
    }
  } else if (isdigit(ch)) {
    num = 12;
    temp_braille = "..0000";
    temp_braille += braille_map[ch + 'a' - '1'];
  }

  strcpy(braille, temp_braille.c_str());

  return num;
}

void encode(const char *text, char *braille) {
  if (!*text) return;
  int count = 0;
  count = encode_character(*text, braille);
  encode(text + 1, braille + count);
}

void print_braille(const char *text, ostream &cout) {
  char braille[512];
  encode(text, braille);
  // print braille
  string braille_1, braille_2, braille_3;
  
  string str_braille(braille);

  int num_braille = str_braille.length()/6;
  for (int i = 0; i < num_braille; i++) {
    braille_1 += str_braille[0 + 6*i];
    braille_1 += str_braille[3 + 6*i];

    braille_2 += str_braille[1 + 6*i];
    braille_2 += str_braille[4 + 6*i];

    braille_3 += str_braille[2 + 6*i];
    braille_3 += str_braille[5 + 6*i];
  }

  for (unsigned int i = 0; i < braille_1.length()/2; i++) {
    cout << braille_1[0 + 2*i] << braille_1[1 + 2*i] << " ";
  }
  cout << endl;
  for (unsigned int i = 0; i < braille_2.length()/2; i++) {
    cout << braille_2[0 + 2*i] << braille_2[1 + 2*i] << " ";
  }
  cout << endl;
  for (unsigned int i = 0; i < braille_3.length()/2; i++) {
    cout << braille_3[0 + 2*i] << braille_3[1 + 2*i] << " ";
  }
  cout << endl;

  // print text
  string str_text(text);
  for (unsigned int i = 0; i < str_text.length(); i++) {
    if (isdigit(str_text[i]) || isupper(str_text[i])) cout << "   ";
    cout << str_text[i] << "  ";
  }
  cout << endl;
}
