#include "parse.h"
#include "repr.h"
#include "str.h"

int main(int argc, const char **argv) {
	while (true) {
		Value v = parse_file(stdin);
		String r = str_append_char(repr(v), '\n');
		str_print(stdout, r);
	}
}
