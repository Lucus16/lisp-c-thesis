#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

#include <stdlib.h>

#include "value.h"

typedef void *String;

String		str_new(const char *content, size_t buf_size); 
String		str_view_new(String base, size_t start, size_t end); 
size_t		str_len(String s); 
const char *str_start(String s); 
char		str_get(String s, size_t index); 
char		str_head(String s); 
String		str_tail(String s); 
String		str_set(String s, size_t index, char value); 
String		str_cat(String dest, String src); 
int			str_cmp(String left, String right); 
size_t		str_find(String haystack, String needle); 

#endif // STR_H_INCLUDED
