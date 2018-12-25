#ifndef BRAILLE_H
#define BRAILLE_H

int encode_character(const char ch, char braille[]);
void encode(const char *text, char *braille);
void print_braille(const char *text, ostream &cout);

#endif