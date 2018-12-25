#ifndef SOUNDEX_H
#define SOUNDEX_H

void encode(const char *, char[]);
int compare(const char *, const char *);
int count(const char * surname, const char * sentence);

#endif