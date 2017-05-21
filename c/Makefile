VG=valgrind --leak-check=full --track-origins=yes --error-exitcode=1
CC=clang -g -std=c11 -Wall -Wextra -Wno-unused -Wno-unused-parameter -Werror
CORE=src/core.h src/value.h src/meta.h src/type.h src/var.h src/ns.h \
	 src/pair.h src/symbol.h src/uniq.h src/str.h src/int.h src/char.h
OBJS=obj/meta.o obj/pair.o obj/symbol.o obj/uniq.o obj/str.o obj/int.o \
	 obj/char.o obj/bool.o obj/free.o obj/init.o obj/ns.o obj/equals.o \
	 obj/repr.o obj/abort.o obj/parse.o obj/var.o

all: $(OBJS)

clean:
	rm obj/*.o

obj/meta.o: src/meta.c src/symbol.h src/value.h src/type.h src/pair.h \
	src/free.h src/bool.h
	$(CC) -c -o $@ $<

obj/pair.o: src/pair.c src/value.h src/type.h src/meta.h
	$(CC) -c -o $@ $<

obj/symbol.o: src/symbol.c src/value.h src/type.h src/pair.h src/meta.h \
	src/abort.h
	$(CC) -c -o $@ $<

obj/uniq.o: src/uniq.c src/value.h src/type.h src/meta.h
	$(CC) -c -o $@ $<

obj/str.o: src/str.c src/value.h src/type.h src/meta.h src/char.h
	$(CC) -c -o $@ $<

obj/int.o: src/int.c
	$(CC) -c -o $@ $<

obj/char.o: src/char.c
	$(CC) -c -o $@ $<

obj/bool.o: src/bool.c
	$(CC) -c -o $@ $<

obj/free.o: src/free.c
	$(CC) -c -o $@ $<

obj/init.o: src/init.c
	$(CC) -c -o $@ $<

obj/equals.o: src/equals.c $(CORE)
	$(CC) -c -o $@ $<

obj/repr.o: src/repr.c $(CORE)
	$(CC) -c -o $@ $<

obj/ns.o: src/ns.c src/value.h src/meta.h src/equals.h
	$(CC) -c -o $@ $<

obj/abort.o: src/abort.c
	$(CC) -c -o $@ $<

obj/parse.o: src/parse.c $(CORE)
	$(CC) -c -o $@ $<

obj/var.o: src/var.c src/value.h src/meta.h
	$(CC) -c -o $@ $<

bin/test_parse_print: src/test_parse_print.c $(OBJS)
	$(CC) -o $@ $< $(OBJS)
