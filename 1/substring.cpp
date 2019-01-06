#include "substring.h"

using namespace std;

bool is_prefix(const char* first, const char* second) {
  if (*first == '\0') return true;

  if (*first == *second) {
    if (is_prefix(first + 1, second + 1)) return true;
  }

  return false;
}

int substring_position(const char* first, const char* second) {
  int count = 0;
  if (!*first) return 0;
  if (!*second) return -1;

  if (is_prefix(first, second)) return count;

  count++;
  count = substring_position(first, second + 1, count);

  return count;
}

int substring_position(const char* first, const char* second, int count) {
  if (!*first) return 0;
  if (!*second) return -1;

  if (is_prefix(first, second)) return count;

  count++;
  count = substring_position(first, second + 1, count);

  return count;
}

int substring_position2(const char* first, const char* second) {
  return (first = strstr(second, first)) ? first - second : -1;
}
