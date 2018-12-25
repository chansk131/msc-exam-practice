#include <iostream>
#include "substring.h"

using namespace std;

int main() {
  cout << substring_position2("this", "this is a simple exercise") << endl;
  cout << substring_position2("is", "this is a simple exercise") << endl;
  cout << substring_position2("is a", "this is a simple exercise") << endl;
  cout << substring_position2("is an", "this is a simple exercise") << endl;
  cout << substring_position2("exercise", "this is a simple exercise") << endl;
  cout << substring_position2("simple exercise", "this is a simple") << endl;
  cout << substring_position2("", "this is a simple exercise") << endl;
  cout << substring_position2("", "") << endl;
  return 0;
}