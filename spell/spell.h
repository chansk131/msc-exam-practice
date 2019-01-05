#include <string>
/* you may assume a maximum word length of 512 characters*/
#define MAX_LENGTH 512
#define NEW_WORD 0
#define RECURSE 1

/* insert your function prototypes here */
int frequency(std::string target);
int edit_distance(const char * a, const char * b);
int edit_distance(const char * a, const char * b, int length_a, int length_b, int cache[MAX_LENGTH][MAX_LENGTH]);
bool spell_correct(const char * word, char * fixed);
int edit_distance_bonus(const char * a, const char * b, int status = 0);