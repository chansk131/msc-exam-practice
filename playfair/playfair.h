#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include <cctype>
#include <string>
#include <iostream>

void prepare(const std::string input, char * output);
void grid(const std::string codeword, char square[6][6]);
void bigram(const char square[6][6], const char inchar1, const char inchar2, char &outchar1, char &outchar2);
void encode(const char square[6][6], const char *prepared, char *encoded);
void decode(const char square[6][6], const char *encoded, char *decoded);

#endif