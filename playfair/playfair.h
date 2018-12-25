#ifndef PLAYFAIR_H
#define PLAYFAIR_H

void prepare(const char * input, char * output);
void grid(const char * codeword, char square[6][6]);
void bigram(char playfair[6][6], const char inchar1, const char inchar2, char &outchar1, char &outchar2);
void encode(char square[6][6], const char * prepared, char encoded[100]);
void decode(char square[6][6], const char * encoded, char * decoded);

#endif