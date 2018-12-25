#include <iostream>

using namespace std;

/* You are supplied with two helper functions */

/* converts a character into a binary string representation */
void ascii_to_binary(char ch, char *binary);

/* converts a binary string representation into a character */
char binary_to_ascii(char *binary);

void ascii_to_binary(char ch, char *binary) {
  for (int n = 128; n; n >>= 1) *binary++ = (ch & n) ? '1' : '0';
  *binary = '\0';
}

char binary_to_ascii(char *binary) {
  int ch = 0;
  for (int n = 0, slide = 128; n < 8; n++, slide >>= 1) {
    if (binary[n] == '1') ch = ch | slide;
  }
  return ch;
}

/* now add your own functions here */
void text_to_binary(const char *text, char *encoded) {
  if (!*text) {
    *encoded = '\0';
    return;
  }
  ascii_to_binary(*text, encoded);
  text_to_binary(text + 1, encoded + 8);
}

void binary_to_text(const char *encoded, char *text) {
  if (!*encoded) {
    *text = '\0';
    return;
  }
  char binary_temp[9];
  for (int i = 0; i < 9; i++) {
    binary_temp[i] = *(encoded + i);
  }
  binary_temp[8] = '\0';
  *text = binary_to_ascii(binary_temp);
  binary_to_text(encoded + 8, text + 1);
}

char pairity(char i, char j, char k) { return (i + j + k) % 2 ? '1' : '0'; }

int pairity_decode(char i, char j, char k, char l) {
  return (i + j + k + l) % 2;
}

void add_error_correction(const char *encoded, char *correct) {
  if (!*encoded) {
    *correct = '\0';
    return;
  }

  char binary_temp[5];
  for (int i = 0; i < 4; i++) {
    binary_temp[i] = *(encoded + i);
  }
  binary_temp[4] = '\0';

  // check pairity

  // c1, c2, d1, c3, d2, d3, d4.
  // c1 = (d 1,2,4)
  *correct = pairity(binary_temp[0], binary_temp[1], binary_temp[3]);
  // c2 = (d 1,3,4)
  *(correct + 1) = pairity(binary_temp[0], binary_temp[2], binary_temp[3]);
  *(correct + 2) = binary_temp[0];
  // c3 = (d 2,3,4)
  *(correct + 3) = pairity(binary_temp[1], binary_temp[2], binary_temp[3]);
  *(correct + 4) = binary_temp[1];
  *(correct + 5) = binary_temp[2];
  *(correct + 6) = binary_temp[3];
  add_error_correction(encoded + 4, correct + 7);
}

int decode(const char *correct, char *decoded, int error_count) {
  if (!*correct) {
    *decoded = '\0';
    return error_count;
  }
  char binary_temp[8];
  for (int i = 0; i < 7; i++) {
    binary_temp[i] = *(correct + i);
  }
  binary_temp[7] = '\0';

  int p1, p2, p3, index = 0;
  p1 = pairity_decode(binary_temp[3], binary_temp[4], binary_temp[5],
                      binary_temp[6]);
  p2 = pairity_decode(binary_temp[1], binary_temp[2], binary_temp[5],
                      binary_temp[6]);
  p3 = pairity_decode(binary_temp[0], binary_temp[2], binary_temp[4],
                      binary_temp[6]);

  if (!(p1 || p2 || p3)) {
    *decoded = binary_temp[2];
    *(decoded + 1) = binary_temp[4];
    *(decoded + 2) = binary_temp[5];
    *(decoded + 3) = binary_temp[6];
  } else {
    index = p1 * 4 + p2 * 2 + p3;
    binary_temp[index - 1] = (binary_temp[index - 1] == '1' ? '0' : '1');
    error_count++;
  }
  *decoded = binary_temp[2];
  *(decoded + 1) = binary_temp[4];
  *(decoded + 2) = binary_temp[5];
  *(decoded + 3) = binary_temp[6];

  return decode(correct + 7, decoded + 4, error_count);
}