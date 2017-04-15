#pragma once

#include <stdbool.h>

typedef struct {} *Char;

Char		char_new(char c);
char		char_get(Char c);
bool		char_less_than(Char a, Char b);
