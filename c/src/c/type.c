#include "../h/type.h"
#include "../h/str.h"

Value type_str(Type type) {
	switch (type) {
		case TYPE_PAIR: return str_lit("pair");
		case TYPE_SYMBOL: return str_lit("symbol");
		case TYPE_UNIQ: return str_lit("unique");
		case TYPE_STRING: return str_lit("string");
		case TYPE_STRING_VIEW: return str_lit("stringview");
		case TYPE_NAMESPACE: return str_lit("namespace");
		case TYPE_VAR: return str_lit("variable");
		case TYPE_FILE_READER: return str_lit("filereader");
		case TYPE_STRING_READER: return str_lit("stringreader");
		case TYPE_CLOSURE: return str_lit("closure");
		case TYPE_PRIMITIVE: return str_lit("primitive");
		case TYPE_SPECIAL_FORM: return str_lit("specialform");
		case TYPE_VAU: return str_lit("vau");
		case TYPE_HANDLER: return str_lit("handler");
		case TYPE_NULL: return str_lit("niltype");
		case TYPE_INT: return str_lit("integer");
		case TYPE_CHAR: return str_lit("character");
		default: return str_format("unknown%i", type);
	}
}
