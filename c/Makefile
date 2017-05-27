VG=valgrind --leak-check=full --track-origins=yes --error-exitcode=1
CC=clang -g -std=c11 -Wall -Wextra -Wno-unused -Wno-unused-parameter -Werror
CS=$(shell ls src/*.c)
HS=$(shell ls src/*.h)
CORE=src/core.h src/value.h src/meta.h src/type.h src/var.h src/ns.h \
	 src/pair.h src/symbol.h src/uniq.h src/str.h src/int.h src/char.h
OBJS=$(CS:src/%.c=obj/%.o)
BINS=bin/repl

all: $(OBJS) $(BINS)

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

obj/reader.o: src/reader.c src/value.h src/meta.h src/str.h
	$(CC) -c -o $@ $<

obj/closure.o: src/closure.c src/value.h src/meta.h src/ns.h
	$(CC) -c -o $@ $<

obj/prim.o: src/prim.c src/value.h src/meta.h
	$(CC) -c -o $@ $<

obj/special.o: src/special.c src/value.h src/meta.h
	$(CC) -c -o $@ $<

obj/eval.o: src/eval.c src/step.h
	$(CC) -c -o $@ $<

obj/constants.o: src/constants.c src/constants.h src/symbol.h src/meta.h src/uniq.h
	$(CC) -c -o $@ $<

obj/match.o: src/match.c src/value.h src/type.h src/ns.h
	$(CC) -c -o $@ $<

obj/repl.o: src/repl.c src/reader.h src/parse.h src/repr.h src/init.h src/str.h
	$(CC) -c -o $@ $<

obj/truthy.o: src/truthy.c src/value.h src/bool.h
	$(CC) -c -o $@ $<

obj/step.o: src/step.c src/step.h src/value.h
	$(CC) -c -o $@ $<

bin/repl: $(OBJS)
	$(CC) -o $@ $(OBJS)
