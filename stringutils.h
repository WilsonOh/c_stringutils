#ifndef STRING_UTILS
#define STRING_UTILS

#include <stddef.h>

/**
 * All documentation is in the souce file
 *
 * @author Wilson Oh
 */
char *firstWord(char *s, char delim);

char *replaceChar(char *s, char toReplace, char toReplaceWith);

char *replace(char *s, char *toReplace, char *toReplaceWith);

char *substring(char *s, size_t start, size_t num_of_chars);

char **split(char *s, char delim, size_t *num_words);

char *strip(char *s);

char *read_line(char *prompt);

char *touppers(char *s);

char *tolowers(char *s);

char *reversed(char *s);

char *read_word(void);

int contains(char *s, char *t);

#endif // !STRING_UTILS
#define STRING_UTILS
