#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "value.h"
#include "char.h"

typedef struct {} *String;

String		str_new(const char *content, size_t size, size_t buf_size); 
String		str_lit(const char *content);
String		str_slice(String base, size_t start, size_t end); 
size_t		str_len(String s); 
char		str_get(String s, size_t index); 
String		str_set(String s, size_t index, char value); 
String		str_append(String left, String right); 
String		str_append_char(String left, char right);
int			str_cmp(String left, String right); 
size_t		str_find(String haystack, String needle); 
Value		str_symbol(String s);
char		str_head(String s);
String		str_tail(String s);

String		str_appendf(String prefix, const char *format, ...);
String		str_printf(const char *format, ...);
void		str_print(FILE *stream, String s);
void		str_println(FILE *stream, String s);
