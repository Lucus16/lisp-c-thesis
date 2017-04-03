#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "type.h"
#include "meta.h"
#include "char.h"

typedef struct String *String;
struct String {
	Meta _meta;
	char *start;
	char *end;
	union {
		String base;
		char *buf_end;
	};
};

size_t power_of_two(size_t s) {
	return 0x8000000000000000 >> (__builtin_clzll(s) + 1);
}

String str_new(const char *content, size_t buf_size) {
	String s = meta_new(TYPE_STRING, sizeof(*s));
	s->start = malloc(sizeof(char) * power_of_two(buf_size));
	s->end = s->start;
	s->buf_end = s->start + buf_size;
	strncpy(s->start, content, buf_size);
	*(s->buf_end) = '\0';
	return s;
}

String str_view_new(String base, size_t start, size_t end) {
	if (meta_ptr_type(base) == TYPE_STRING_VIEW && meta_is_single_ref(base)) {
		base->end = base->start + end;
		base->start += start;
		return base;
	}
	String s = meta_new(TYPE_STRING_VIEW, sizeof(*s));
	if (meta_ptr_type(base) == TYPE_STRING) {
		s->base = base;
	} else {
		s->base = base->base;
	}
	s->start = base->start + start;
	s->end = base->start + end;
	return s;
}

Value str_free(String s) {
	free(s->start);
	free(s);
	return NULL;
}

Value str_view_free(String s) {
	Value rest = s->base;
	free(s);
	return rest;
}

size_t str_len(String s) {
	return s->end - s->start;
}

const char *str_start(String s) {
	return s->start;
}

Char str_get(String s, size_t index) {
	return char_new(*(s->start + index));
}

Char str_head(String s) {
	return char_new(*(s->start));
}

String str_tail(String s) {
	return str_view_new(s, 1, s->end - s->start);
}

String str_set(String s, size_t index, Char value) {
	if (!meta_is_single_ref(s) || (meta_ptr_type(s) == TYPE_STRING_VIEW &&
				!meta_is_single_ref(s->base))) {
		s = str_new(s->start, s->end - s->start);
	}
	*(s->start + index) = char_get(value);
	return s;
}

String str_cat(String dest, String src) {
	if ((!meta_is_single_ref(dest) || (meta_ptr_type(dest) == TYPE_STRING_VIEW &&
				!meta_is_single_ref(dest->base))) ||
			(dest->buf_end - dest->end < src->end - src->start)) {
		dest = str_new(dest->start, str_len(dest) + str_len(src));
	}
	strncpy(dest->end, src->start, str_len(src));
	return dest;
}

int str_cmp(String left, String right) {
	size_t min_len = str_len(left) < str_len(right) ? str_len(left) : str_len(right);
	int result = strncmp(left->start, right->start, min_len);
	if (result == 0) {
		if (str_len(left) > str_len(right)) {
			return 1;
		} else if (str_len(left) < str_len(right)) {
			return -1;
		} else {
			return 0;
		}
	} else {
		return result;
	}
}

size_t str_find(String haystack, String needle) {
	for (char *a = haystack->start; a < haystack->end - str_len(needle); a++) {
		if (strncmp(a, needle->start, str_len(needle)) == 0) {
			return a - haystack->start;
		}
	}
	return -1;
}
