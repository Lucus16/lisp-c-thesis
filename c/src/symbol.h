#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED

typedef struct {} *Symbol;

Symbol		symbol_new(const char *name, size_t len);
Symbol		unique_new(const char *name);
const char *symbol_name(Symbol symbol);

#endif // SYMBOL_H_INCLUDED
