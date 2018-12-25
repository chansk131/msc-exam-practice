#ifndef CORRECT_H
#define CORRECT_H

void ascii_to_binary(char letter, char *output);
char binary_to_ascii(char *binary);
void text_to_binary(const char * text, char *encoded);
void binary_to_text(const char * encoded, char * text);
void add_error_correction(const char * encoded, char * correct);
int decode(const char * correct, char * decoded, int error_count = 0);

#endif