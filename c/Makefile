VG=valgrind --leak-check=full --track-origins=yes --error-exitcode=1
CC=clang -g -std=c11 -Wall -Wextra -Wno-unused -Wno-unused-parameter -Werror
SOURCES=$(shell ls src/*.c)
HEADERS=$(shell ls src/*.h)
DEPENDS=$(SOURCES:src/%.c=dep/%.d)
OBJECTS=$(SOURCES:src/%.c=obj/%.o)

BINS=bin/repl

all: $(OBJECTS) $(BINS)

clean:
	rm obj/*.o
	rm dep/*.d

depend: $(DEPENDS)

dep/%.d: src/%.c
	$(CC) -MM $< -MT '$(@:dep/%.d=obj/%.o)' -MF $@

obj/%.o: src/%.c
	$(CC) -c -o $@ $<

#bin/%: obj/%.o
#	$(CC) -o $@ 

bin/repl: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

include $(DEPENDS)
