/* you may assume a maximum word length of 512 characters*/
#define MAX_LENGTH 512

/* insert your function prototypes here */
int frequency(const char * target);
int edit_distance(const char * a, const char * b);
int edit_distance(const char * a, const char * b, int length_a, int length_b);
bool spell_correct(const char * word, char * fixed);