VG=valgrind --leak-check=full --track-origins=yes --error-exitcode=1
CC=clang -g -std=c11 -Wall -Wextra -Wno-unused -Wno-unused-parameter -Werror

all: obj/meta.o obj/pair.o obj/symbol.o obj/uniq.o obj/str.o obj/int.o \
	obj/char.o obj/bool.o obj/free.o obj/init.o obj/exit.o obj/print.o

obj/meta.o: src/meta.c src/symbol.h src/value.h src/type.h src/pair.h src/free.h
	$(CC) -c -o $@ $<

obj/pair.o: src/pair.c src/value.h src/type.h src/meta.h src/print.h
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

obj/free.o: src/free.c
	$(CC) -c -o $@ $<

obj/init.o: src/init.c
	$(CC) -c -o $@ $<

obj/exit.o: src/exit.c
	$(CC) -c -o $@ $<

obj/print.o: src/print.c src/value.h src/meta.h src/type.h
	$(CC) -c -o $@ $<
