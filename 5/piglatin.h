#ifndef PIGLATIN_H
#define PIGLATIN_H

bool isVowel(char ch, int position, int length);
int findFirstVowel(const char * text);
void translateWord(const char * text, char piglatin[]);
void get_word(std::ifstream& input, char text[]);
void translateStream(std::ifstream &input, std::ostream &cout);

#endif