all: obj/meta.o obj/pair.o obj/symbol.o

obj/meta.o: src/meta.c src/symbol.h src/value.h src/type.h src/pair.h
	$(CC) -c -o $@ $<

obj/pair.o: src/pair.c src/value.h src/meta.h
	$(CC) -c -o $@ $<

obj/symbol.o: src/symbol.c src/value.h src/pair.h src/meta.h
	$(CC) -c -o $@ $<
