#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include <string>
#include <algorithm>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */
int leading_zeros(const char * digest) {        
  int count = 0;
  bool is_leading_zeros = true;
  for (int i = 0; digest[i]; i++) {
    if (digest[i] == '0' && is_leading_zeros) {
      count++;
    } else {
      is_leading_zeros = false;
    }
    if (!isxdigit(digest[i])) return -1;
  }

  return count;
}

bool file_to_SHA1_digest(const char * filename, char * digest) {
  ifstream in;
  in.open(filename);

  if (!in) {
    strcpy(digest, "error");
    return false;
  }

  string message;
  char ch;

  in.get(ch);
  while (!in.eof()) {
    message += ch;
    in.get(ch);
  }

  in.close();

  text_to_SHA1_digest(message.c_str(), digest);
  return true;
}

string make_str_header(const char * recipient, const char * filename, int count, const char * digest) {
  string header;
  header += recipient;
  header += ':';
  header += digest;
  header += ':';
  header += to_string(count);
  return header;
}

bool make_header(const char * recipient, const char * filename, char * header) {

  char digest[41];
  char new_digest[41];
  long long count = 0;

  if (!file_to_SHA1_digest(filename, digest)) return false;

  string str_header;
  str_header = make_str_header(recipient, filename, count, digest);

  text_to_SHA1_digest(str_header.c_str(), new_digest);

  while (leading_zeros(new_digest) < 5) {
    count++;
    if (count > 10000000) return false;
    text_to_SHA1_digest(str_header.c_str(), new_digest);
    str_header = make_str_header(recipient, filename, count, digest);
  }

  strcpy(header, str_header.c_str());
  return true;
}

MessageStatus check_header(const char * recipient, const char * header, const char * filename) {
  // check recipient

  string str_header(header);
  int field_count = count(str_header.begin(), str_header.end(), ':') + 1;
  if (field_count != 3) return INVALID_HEADER;

  string temp_recipient;
  string temp_digest;
  string temp_count;

  int i = 0;
  while(header[i] && header[i] != ':') {
    temp_recipient += header[i];
    i++;
  }
  if (strcmp(recipient, temp_recipient.c_str())) return WRONG_RECIPIENT;

  // check digest
  i++;
  while(header[i] && header[i] != ':') {
    temp_digest += header[i];
    i++;
  }
  char file_digest[41];
  file_to_SHA1_digest(filename, file_digest);
  cout << file_digest << " " << temp_digest << endl;
  if (strcmp(file_digest, temp_digest.c_str())) return INVALID_MESSAGE_DIGEST;

  i++;
  while(header[i]) {
    temp_count += header[i];
    i++;
  }
  int count = stoi(temp_count);

  char ref_header[512];
  if (make_header(recipient, filename, ref_header)) {
    if (strcmp(ref_header, header)) return INVALID_HEADER_DIGEST;
  }

  return VALID_EMAIL;
}