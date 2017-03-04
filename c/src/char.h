#ifndef CHAR_H_INCLUDED
#define CHAR_H_INCLUDED

#include <stdbool.h>

typedef void *Char;

Char		char_new(char c);
char		char_get(Char c);
bool		char_less_than(Char a, Char b);

#endif // CHAR_H_INCLUDED
