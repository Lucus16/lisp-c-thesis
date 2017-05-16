#pragma once

typedef enum {
	TYPE_PAIR = 1,
	TYPE_SYMBOL,
	TYPE_UNIQ,
	TYPE_STRING,
	TYPE_STRING_VIEW,
	TYPE_NAMESPACE,
	TYPE_VAR,
	TYPE_NEXT_UNUSED,
	TYPE_NULL = 0x1000000,
	TYPE_INT,
	TYPE_CHAR,
} Type;
