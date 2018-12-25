#include <string>
#include <cctype>

using namespace std;

void reverse(const string word, char reversed[])
{
  int end = word.length();
  reversed[end - 1] = '\0';

  for (int i = 0; i < end; i++)
  {
    reversed[end - i - 1] = word[i];
  }
}

int compare(const char * one, const char * two)
{
  // is alpha
  int i = 0;
  while (*(one + i) && !isalpha(*(one + i))) i++;

  int j = 0;
  while (*(two + j) && !isalpha(*(two + j))) j++;

  if (!*(one + i) && !*(two + j)) return 1;
  if (!(*(one + i) && *(two + j))) return 0;

  if (toupper(*(one + i)) != toupper(*(two + j))) return 0;
  return compare(one + i + 1, two + j + 1);
}

int palindrome(const char * sentence)
{
  // reverse
  string str(sentence);
  char reversed[str.length()];

  reverse(str, reversed);

  // compare
  return compare (sentence, reversed);
}

string sort_str(string str)
{
  string sorted;

  for (int i = 0; i < str.length(); i++)
  {
    if (!isalpha(str[i])) continue;
    if (i > 0)
    {
      if (toupper(str[i]) > sorted.back()) {
        sorted += toupper(str[i]);
      }
      else {
        for (int j = 0; j < sorted.length(); j++) {
          if (sorted[j] > toupper(str[i]))
          {
            sorted.insert(sorted.begin()+j, toupper(str[i]));
            break;
          }
        }
      }
    } else {
      sorted += toupper(str[i]);
    }
  }
  return sorted;
}

int anagram(const string str1, const string str2)
{
  string sorted_str1, sorted_str2;

  sorted_str1 = sort_str(str1);
  sorted_str2 = sort_str(str2);

  if (sorted_str1 == sorted_str2) return 1;
  else return 0;
}