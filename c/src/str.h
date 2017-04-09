#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

#include <stdlib.h>

#include "value.h"
#include "char.h"

typedef struct {} *String;

String		str_new(const char *content, size_t size, size_t buf_size); 
String		str_slice(String base, size_t start, size_t end); 
size_t		str_len(String s); 
char		str_get(String s, size_t index); 
String		str_set(String s, size_t index, char value); 
String		str_append(String left, String right); 
int			str_cmp(String left, String right); 
size_t		str_find(String haystack, String needle); 

char		str_head(String s) {
	return str_get(s, 0);
}

String		str_tail(String s) {
	return str_slice(s, 1, str_len(s));
}

#endif // STR_H_INCLUDED
