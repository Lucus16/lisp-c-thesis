VG=valgrind --leak-check=full --track-origins=yes --error-exitcode=1
CC=clang -g -std=c11 -Wall -Wextra -Wno-unused -Wno-unused-parameter -Werror

all: obj/meta.o obj/pair.o obj/symbol.o obj/uniq.o obj/str.o obj/int.o \
	obj/char.o obj/bool.o

obj/meta.o: src/meta.c src/symbol.h src/value.h src/type.h src/pair.h
	$(CC) -c -o $@ $<

obj/pair.o: src/pair.c src/value.h src/type.h src/meta.h
	$(CC) -c -o $@ $<

obj/symbol.o: src/symbol.c src/value.h src/type.h src/pair.h src/meta.h
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
