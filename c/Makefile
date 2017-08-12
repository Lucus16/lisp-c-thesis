VG=valgrind --leak-check=full --track-origins=yes --error-exitcode=1
CC=clang -g -std=c11 -Wall -Wextra -Wno-unused -Wno-unused-parameter -Werror
CS=$(wildcard src/c/*.c)
BS=$(wildcard src/b/*.c)
TS=$(wildcard src/t/*.c)
ALLCS=$(CS) $(BS) $(TS)
DEPENDS=$(ALLCS:src/%.c=dep/%.d)
OBJECTS=$(CS:src/c/%.c=obj/%.o)
BINS=$(BS:src/b/%.c=bin/%)
TESTS=$(TS:src/t/%.c=test/%)
SHELLTESTS=$(wildcard src/t/*.sh)

all: $(OBJECTS) $(BINS) $(TESTS)

clean:
	rm -r obj/ dep/ bin/ test/ || true
	mkdir -p obj/ dep/{d,b,c,t} bin/ test/

depend: $(DEPENDS)

debug-%: bin/%
	rlwrap $(VG) $<

dep/c/%.d: src/c/%.c
	$(CC) -MM $< -MT '$(@:dep/c/%.d=obj/%.o)' -MF $@

dep/b/%.d: src/b/%.c
	$(CC) -MM $< -MT '$(@:dep/b/%.d=bin/%)' -MF $@

dep/t/%.d: src/t/%.c
	$(CC) -MM $< -MT '$(@:dep/t/%.d=test/%)' -MF $@

obj/%.o: src/c/%.c
	$(CC) -c -o $@ $<

bin/%: src/b/%.c $(OBJECTS)
	$(CC) -o $@ $< $(OBJECTS)

test/%: src/t/%.c $(OBJECTS)
	$(CC) -o $@ $< $(OBJECTS)

include $(DEPENDS)
