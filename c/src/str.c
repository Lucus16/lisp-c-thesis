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
	return 0x8000000000000000 >> (__builtin_clzll(s - 1) - 1);
}

size_t max(size_t a, size_t b) {
	return a > b ? a : b;
}

size_t min(size_t a, size_t b) {
	return a < b ? a : b;
}

String get_base(String s) {
	if (meta_ptr_type(s) == TYPE_STRING) {
		return s;
	} else {
		return s->base;
	}
}

String str_new(const char *start, size_t size, size_t buf_size) {
	String s = meta_new(TYPE_STRING, sizeof(*s));
	s->start = malloc(sizeof(char) * buf_size);
	s->end = s->start + size;
	s->buf_end = s->start + buf_size;
	memcpy(s->start, start, size);
	return s;
}

String str_view_new(String base, size_t start, size_t end) {
	if (meta_ptr_type(base) == TYPE_STRING_VIEW && meta_is_single_ref(base)) {
		base->end = base->start + end;
		base->start += start;
		return base;
	}
	String s = meta_new(TYPE_STRING_VIEW, sizeof(*s));
	s->base = get_base(base);
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

char str_get(String s, size_t index) {
	return s->start[index];
}

Char str_head(String s) {
	return char_new(*(s->start));
}

String str_tail(String s) {
	return str_view_new(s, 1, s->end - s->start);
}

String str_set(String s, size_t index, char value) {
	if (!meta_is_single_ref(s) || (meta_ptr_type(s) == TYPE_STRING_VIEW &&
				!meta_is_single_ref(s->base))) {
		s = str_new(s->start, s->end - s->start, s->end - s->start);
	}
	s->start[index] = value;
	return s;
}

int str_cmp(String left, String right) {
	size_t min_len = min(str_len(left), str_len(right));
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
	size_t needle_len = str_len(needle);
	char *end = haystack->end - needle_len;
	for (char *a = haystack->start; a < end; a++) {
		if (strncmp(a, needle->start, needle_len) == 0) {
			return a - haystack->start;
		}
	}
	return -1;
}

size_t buf_space(String s) {
	String base = get_base(s);
	if (meta_is_single_ref(s) && meta_is_single_ref(base)) {
		return base->buf_end - s->end;
	} else {
		return 0;
	}
}

/*
 * buf_alloc makes space in the buffer of String or StringView s for at least
 * size more characters. Since Strings and StringViews are immutable, this means
 * a new buffer is allocated and the contents copied if there wasn't already
 * enough space or if the references weren't unique. Because one alloc is likely
 * to be followed by another, when used in append for example, extra space may
 * be allocated to allow for cheap future allocations.
 */
String buf_alloc(String s, size_t size) {
	if (buf_space(s) < size) {
		size_t s_len = str_len(s);
		size_t new_size = max(power_of_two(s_len + 1), s_len + size);
		s = str_new(s->start, s_len, new_size);
	}
	return s;
}

String str_append(String dest, String src) {
	dest = buf_alloc(dest, str_len(src));
	memcpy(dest->end, src->start, str_len(src));
	dest->end += str_len(src);
	return dest;
}
