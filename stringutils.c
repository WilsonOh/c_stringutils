#include "stringutils.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/**
 * Library of string utility functions
 * It is the user's responsibility to free the memory allocated
 * by the functions
 *
 * @author Wilson Oh
 * @version 1.0.0 20th April 2022
 */


/**
 * Takes in a string and returns the first word in the string, separated by a
 * delimeter If there is only one word in the string, the same string is
 * returned
 *
 * @param s the string to be processed
 * @param delim the delimeter character
 *
 * @return the first word of the string, or the string itself if there is only
 * one word
 */
char *firstWord(char *s, char delim) {
  char *ret = malloc(sizeof(char) * strlen(s) + 1);
  char *index;
  if ((index = memccpy(ret, s, delim, strlen(s) + 1)) == NULL) {
    return s;
  }
  *(index - 1) = 0;
  return ret;
}

/**
 * Takes in a string and returns the an uppercase version of it
 *
 * @param s string to be uppercased
 *
 * @return the uppercase string
 */
char *touppers(char *s) {
  char *ret = malloc(sizeof(char) * strlen(s) + 1);
  for (size_t i = 0; i < strlen(s); i++) {
    ret[i] = toupper(s[i]);
  }
  ret[strlen(s)] = 0;
  return ret;
}

/**
 * Takes in a string and returns the an lowercase version of it
 *
 * @param s string to be lowercased
 *
 * @return the lowercase string
 */
char *tolowers(char *s) {
  char *ret = malloc(sizeof(char) * strlen(s) + 1);
  for (size_t i = 0; i < strlen(s); i++) {
    ret[i] = tolower(s[i]);
  }
  ret[strlen(s)] = 0;
  return ret;
}

/**
 * Splits a string by the specified delimiting character
 *
 * @param s string to be split
 * @param delim the delimiting character
 * @param num_words pointer to a counter with holds the number of words recieved from the split
 *
 * Example: 
 *
 * size_t count;
 * char **words = split("Hello World", ' ', &count);
 * for (size_t i = 0; i < count; i ++) {
 *   ...
 * }
 *
 * @return an array of of num_words number of strings 
 */
char **split(char *s, char delim, size_t *num_words) {
  char **ret = malloc(sizeof(char *));
  char **tmp;
  size_t count = 1;
  size_t idx = 0;
  for (size_t i = 0; i < strlen(s) + 1; i++) {
    if (s[i] == delim || s[i] == '\0') {
      ret[count - 1] = substring(s, idx, i - idx);
      idx = ++i;
      tmp = realloc(ret, (count + 1) * sizeof(char *));
      ret = tmp;
      count++;
    }
  }
  *num_words = count - 1;
  return ret;
}

/**
 * Replace all instances of a character with another character in a string
 *
 * @param s string to be processed
 * @param toReplace character in string to be replaced
 * @param toReplaceWith character to replace with
 *
 * @param the modified string
 */
char *replaceChar(char *s, char toReplace, char toReplaceWith) {
  char *ret = malloc(sizeof(char) * strlen(s) + 1);
  for (size_t i = 0; i < strlen(s); i++) {
    if (s[i] == toReplace) {
      ret[i] = toReplaceWith;
    } else {
      ret[i] = s[i];
    }
  }
  ret[strlen(s)] = 0;
  return ret;
}

/**
 * Convenience function which is a wrapper around POSIX getline.
 * The function takes in a string prompt, which will be printed out
 * before taking in user input if it is not an empty string
 *
 * @param prompt prompt to be printed out before getting user input. Pass in an empty string "" to not print out anything
 *
 * @return the user input without the trailing newline
 */
char *input(char *prompt) {
  errno = 0;
  if (prompt != NULL) {
    printf("%s", prompt);
  }
  char *nptr = NULL;
  size_t dummy = 0;
  ssize_t count = getline(&nptr, &dummy, stdin);
  if (count == -1) {
    fprintf(stderr, "getline: Unable to read line\n");
  }
  if (errno == EINVAL) {
    fprintf(stderr, "getline: Bad Arguments\n");
  }
  if (errno == ENOMEM) {
    fprintf(stderr, "getline: Memory allocation failed\n");
  }
  // remove the trailing newline
  nptr[count - 1] = 0;
  return nptr;
}

/**
 * returns a substring of an original string
 *
 * @param s the original string
 * @param start the starting index
 * @param num_of_chars the number of characters after the starting index to be extracted
 *
 * @return the substring of s starting at start and ending at start + num_of_chars
 */
char *substring(char *s, size_t start, size_t num_of_chars) {
  if (start > strlen(s) || start + num_of_chars > strlen(s)) {
    return NULL;
  }
  char *ret = malloc(sizeof(char) * num_of_chars + 1);
  memcpy(ret, s + start, num_of_chars);
  ret[num_of_chars + 1] = 0;
  return ret;
}

/**
 * Removes ALL whitespace in a string (yeah it is not very useful for multi-word strings)
 *
 * @param s the string to be processed
 *
 * @return the string with all whitespace removed
 */
char *strip(char *s) {
  char *buffer = malloc(sizeof(char));
  char *tmp;
  size_t idx = 0;
  while (*s) {
    if (!isspace(*s)) {
      buffer[idx++] = *s;
    }
    s++;
    tmp = realloc(buffer, idx * sizeof(char));
    buffer = tmp;
  }
  buffer[idx] = 0;
  return buffer;
}

static bool _strcmp(char *s, char *toReplace, int index) {
  for (size_t i = 0; i < strlen(toReplace); i++) {
    if (s[index + i] != toReplace[i])
      return false;
  }
  return true;
}

/**
 * Replace all instances of a substring with another string
 *
 * @param s string to be processed
 * @param toReplace substring in string to be replaced
 * @param toReplaceWith string to replace with
 *
 * @param the modified string
 */
char *replace(char *s, char *toReplace, char *toReplaceWith) {
  char *buf = malloc(sizeof(char));
  char *tmp;
  size_t string_index = 0;
  size_t buffer_index = 0;
  while (string_index <= strlen(s)) {
    if (s[string_index] == *toReplace && _strcmp(s, toReplace, string_index)) {
      memcpy(&buf[buffer_index], toReplaceWith, strlen(toReplaceWith));
      string_index += strlen(toReplace);
      buffer_index += strlen(toReplaceWith);
    } else {
      buf[buffer_index++] = s[string_index++];
    }
    tmp = realloc(buf, buffer_index * sizeof(char));
    buf = tmp;
  }
  buf[buffer_index] = 0;
  return buf;
}

/**
 * Returns a copy of the string which is reversed
 *
 * @param s the string to be reversed
 *
 * @return the reversed string
 */
char *reversed(char *s) {
  char tmp;
  size_t len = strlen(s) - 1;
  char *ret = strdup(s);
  for (size_t i = 0; i <= len/2; i++) {
    tmp = ret[i];
    ret[i] = ret[len - i];
    ret[len - i] = tmp;
  }
  return ret;
}
