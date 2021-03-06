#include "stringutils.h"
#include "iterator.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Library of string utility functions
 * It is the user's responsibility to free the memory allocated
 * by the functions
 *
 * @author Wilson Oh
 * @version 1.0.0 20th April 2022
 */

/**
 * @brief Helper function for replace
 */
static bool compare_substrings(char *s, char *toReplace, size_t index) {
  for (size_t i = 0; i < strlen(toReplace); i++) {
    if (s[index + i] == 0 || s[index + i] != toReplace[i])
      return false;
  }
  return true;
}

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
    ret[i] = (char)toupper(s[i]);
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
    ret[i] = (char)tolower(s[i]);
  }
  ret[strlen(s)] = 0;
  return ret;
}

/**
 * Splits a string by the specified delimiting character
 * I guess the difference between this and string.h's strsep is
 * that strsep is destruction while this function is not.
 *
 * @param s string to be split
 * @param delim the delimiting string
 * @param num_words pointer to a counter with holds the number of words recieved
 * from the split
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
char **split(char *s, char *delim, size_t *num_words) {
  char **ret = malloc(sizeof(char *));
  char **tmp;
  size_t count = 1;
  size_t idx = 0;
  size_t delim_len = strlen(delim);
  for (size_t i = 0; i <= strlen(s) + 1; i++) {
    if (s[i] == *delim) {
      if (compare_substrings(s, delim, i)) {
        ret[count - 1] = substring(s, idx, i - idx);
        i += delim_len - 1;
        idx = i + 1;
        tmp = realloc(ret, (count + 1) * sizeof(char *));
        ret = tmp;
        count++;
      }
    } else if (s[i] == '\0') {
      char *x;
      if (strlen((x = substring(s, idx, i - idx))) != 0) {
        ret[count - 1] = x;
      } else {
        free(x);
      }
      i += delim_len;
    }
  }
  *num_words = count;
  return ret;
}

/**
 * Same as split, but returns a string_iterator_t type instead
 * This aims to make iterating over the tokens more convenient as
 * there is no need to manage the counting using a separate variable
 *
 * Example Usage:
 *
 * string_iterator_t *si = split_iter("Hello World", ' ');
 * char *curr;
 * while ((curr = iterate(si)) != NULL) {
 *   printf("%s\n", curr);
 *   etc...
 * }
 *
 * @param s the string to be split
 * @param delim the delimeter character
 *
 * @return a pointer to a string_iterator_t which contains the split tokens
 */

string_iterator_t *split_iter(char *s, char *delim) {
  char **ret = malloc(sizeof(char *));
  char **tmp;
  size_t count = 1;
  size_t idx = 0;
  size_t delim_len = strlen(delim);
  for (size_t i = 0; i <= strlen(s) + 1; i++) {
    if (s[i] == *delim) {
      if (compare_substrings(s, delim, i)) {
        ret[count - 1] = substring(s, idx, i - idx);
        i += delim_len - 1;
        idx = i + 1;
        tmp = realloc(ret, (count + 1) * sizeof(char *));
        ret = tmp;
        count++;
      }
    } else if (s[i] == '\0') {
      char *x;
      if (strlen((x = substring(s, idx, i - idx))) != 0) {
        ret[count - 1] = x;
      } else {
        free(x);
      }
      i += delim_len;
    }
  }
  string_iterator_t *si = new_string_iterator(ret, count);
  return si;
}

/**
 * Replace all instances of a character with another character in a string
 *
 * @param s string to be processed
 * @param toReplace character in string to be replaced
 * @param toReplaceWith character to replace with
 *
 * @return the modified string
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
 * @param prompt prompt to be printed out before getting user input. Pass in an
 * empty string "" to not print out anything
 *
 * @return the user input without the trailing newline
 */
char *read_line(char *prompt) {
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
 * @param num_of_chars the number of characters after the starting index to be
 * extracted
 *
 * @return the substring of s starting at start and ending at start +
 * num_of_chars
 */
char *substring(char *s, size_t start, size_t num_of_chars) {
  if (start > strlen(s) || start + num_of_chars > strlen(s)) {
    return NULL;
  }
  char *ret = malloc(sizeof(char) * num_of_chars + 1);
  memcpy(ret, s + start, num_of_chars);
  ret[num_of_chars] = 0;
  return ret;
}

/**
 * Removes ALL whitespace in a string (yeah it is not very useful for multi-word
 * strings)
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

/**
 * Replace all instances of a substring with another string
 *
 * @param s string to be processed
 * @param toReplace substring in string to be replaced
 * @param toReplaceWith string to replace with
 *
 * @return the modified string
 */
char *replace(char *s, char *toReplace, char *toReplaceWith) {
  char *buf = malloc(sizeof(char));
  char *tmp;
  size_t string_index = 0;
  size_t buffer_index = 0;
  size_t curr;
  while (string_index <= strlen(s)) {
    if (s[string_index] == *toReplace &&
        compare_substrings(s, toReplace, string_index)) {
      curr = buffer_index;
      string_index += strlen(toReplace);
      buffer_index += strlen(toReplaceWith);
      tmp = realloc(buf, buffer_index * sizeof(char) + 1);
      buf = tmp;
      memcpy(&buf[curr], toReplaceWith, strlen(toReplaceWith));
    } else {
      curr = buffer_index;
      buf[buffer_index++] = s[string_index++];
      tmp = realloc(buf, buffer_index * sizeof(char) + 1);
      buf = tmp;
    }
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
  for (size_t i = 0; i <= len / 2; i++) {
    tmp = ret[i];
    ret[i] = ret[len - i];
    ret[len - i] = tmp;
  }
  return ret;
}

static char skip_space() {
  int c;
  do {
    c = fgetc(stdin);
  } while (isspace(c) && c != EOF);
  return (char)c;
}

static size_t fill_buffer(char *buffer, size_t buf_size) {
  int c;
  size_t i = 0;
  while (1) {
    c = fgetc(stdin);
    if (isspace(c) || c == EOF) {
      buffer[i] = 0;
      return i;
    }
    buffer[i] = (char)c;
    if (i++ == buf_size) {
      return i;
    }
  }
}

/**
 * Reads from the standard input and stops upon hitting whitespace, skipping any
 * leading whitespace
 *
 * @return the word read
 */
char *read_word() {
  char *ret = malloc(sizeof(char));
  if (ret == NULL) {
    fprintf(stderr, "read_word: memory allocation error\n");
    return NULL;
  }
  char *tmp;
  int c = skip_space();
  if (c == EOF) {
    free(ret);
    fprintf(stderr, "read_word: EOF error\n");
    return NULL;
  }
  *ret = (char)c;
  size_t buf_size = 30;
  size_t len;
  size_t total_len = 1;
  char buffer[buf_size];
  do {
    len = fill_buffer(buffer, buf_size);
    tmp = realloc(ret, total_len + len);
    if (tmp == NULL) {
      fprintf(stderr, "read_word: memory allocation error\n");
      return NULL;
    }
    ret = tmp;
    memcpy(ret + total_len, buffer, len);
    total_len += len;
  } while (len == buf_size && buffer[len - 1] != '\0');
  return ret;
}

/**
 * Checks if a string contains a substring (case sensitive)
 *
 * @param s the string
 * @param t the substring to be checked
 *
 * @return 0 if the string s contains the substring t, else 1
 */
int contains(char *s, char *t) {
  for (size_t i = 0; i < strlen(s); i++) {
    if (s[i] == *t) {
      if (compare_substrings(s, t, i)) {
        return 1;
      }
    }
  }
  return 0;
}

/**
 * Converts a long integer to its string representation
 *
 * @param n the long integer to be converted
 *
 * @return the long integer's string representation
 */
char *long_to_string(long n) {
  char *ret = malloc(1);
  size_t count = 1;
  while (n) {
    ret[count - 1] = (n % 10) + '0';
    ret = realloc(ret, count++);
    n /= 10;
  }
  ret[count] = 0;
  char *rev = reversed(ret);
  free(ret);
  return rev;
}

/**
 * Reads from a file and maps its entire contents in memory, returning a pointer
 * to it
 *
 * @param filepath path to the file to be read
 * @return pointer to the contents of the file mapped in memory
 *
 */
char *read_file(const char *filepath) {
  ssize_t fd;
  char *path;
  char *ret;
  struct stat sb;
  if ((path = realpath(filepath, NULL)) == NULL) {
    if (errno == ENOENT) {
      fprintf(stderr, "Error: File <%s> Does Not Exist\n", filepath);
    }
  }
  fd = open(path, O_RDONLY);
  if (fstat(fd, &sb) == -1) {
    perror("fstat");
  }
  if ((ret = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
      MAP_FAILED) {
    perror("mmap");
  }
  free(path);
  return ret;
  // munmap(file, sb.st_size);
}

/**
 * Wrapper around strtol
 * Out of range, invalid input and incomplete conversion errors are
 * checked for.
 *
 * @param s string to be converted to a long
 * @return the long representation of the string if conversion is successful
 * else returns LONG_MAX
 */
long string_to_long(char *s) {
  char *end_ptr;
  long number;

  errno = 0;

  number = strtol(s, &end_ptr, 10);

  if (ERANGE == errno) {
    fprintf(stderr, "string_to_long: number '%s' out of range\n", s);
    return LONG_MAX;
  }
  if (end_ptr == s) {
    fprintf(stderr, "string_to_long: '%s' is not a valid numeric input\n", s);
    return LONG_MAX;
  }
  if ('\n' != *end_ptr && '\0' != *end_ptr) {
    fprintf(
        stderr,
        "string_to_long: reach the end without null/newline. '%s' remains\n",
        end_ptr);
    return LONG_MAX;
  }
  return number;
}

/**
 * Returns the index of a substring `needle` in another string `haystack`.
 * If the substring doesn't exist, return -1
 *
 * @param haystack the string to be searched
 * @param needle the substring
 *
 * @return index of `needle` in `haystack` if found, else -1
 */
int find_substr(char *haystack, char *needle) {
  size_t haystack_len = strlen(haystack);
  size_t needle_len = strlen(needle);
  for (size_t i = 0; i < haystack_len - needle_len + 1; ++i) {
    if (compare_substrings(haystack, needle, i)) {
      return i;
    }
  }
  return -1;
}
