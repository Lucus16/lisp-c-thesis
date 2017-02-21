#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

typedef enum {
	TYPE_PAIR,
	TYPE_NEXT_UNUSED,
	TYPE_MASK = 0xffffff,
	TYPE_NULL,
} Type;

#endif // TYPE_H_INCLUDED
