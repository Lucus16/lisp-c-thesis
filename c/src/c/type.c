#include "../h/type.h"
#include "../h/str.h"

Value type_str(Type type) {
	switch (type) {
		case TYPE_PAIR: return str_lit("Pair");
		case TYPE_SYMBOL: return str_lit("Symbol");
		case TYPE_UNIQ: return str_lit("Unique");
		case TYPE_STRING:
		case TYPE_STRING_VIEW: return str_lit("String");
		case TYPE_NAMESPACE: return str_lit("Namespace");
		case TYPE_VAR: return str_lit("Variable");
		case TYPE_FILE_READER:
		case TYPE_STRING_READER: return str_lit("Reader");
		case TYPE_CLOSURE: return str_lit("Closure");
		case TYPE_PRIMITIVE: return str_lit("Primitive");
		case TYPE_SPECIAL_FORM: return str_lit("Specialform");
		case TYPE_VAU: return str_lit("Vau");
		case TYPE_HANDLER: return str_lit("Handler");
		case TYPE_NULL: return str_lit("Nil");
		case TYPE_INT: return str_lit("I61");
		case TYPE_CHAR: return str_lit("Character");
		default: return str_format("Unknown%x", type);
	}
}
