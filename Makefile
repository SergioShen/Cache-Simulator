CC=g++
CFLAGS=-w -O2
LEVEL=MONO_LEVEL
all: sim

sim: main.o cache.o memory.o config.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: cache.h memory.h config.h cache.cpp memory.cpp config.cpp main.cpp
	$(CC) $(CFLAGS) -D $(LEVEL) -c main.cpp

cache.o: cache.h storage.h utility.h cache.cpp
	$(CC) $(CFLAGS) -c cache.cpp

memory.o: memory.h storage.h memory.cpp
	$(CC) $(CFLAGS) -c memory.cpp

config.o: config.h utility.h config.cpp
	$(CC) $(CFLAGS) -c config.cpp

.PHONY: clean

clean:
	rm -rf sim *.o
