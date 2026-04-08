CC = g++
CFLAGS = --std=c++20 -Wall -pedantic -g -Werror
LIB = -lncurses -lpanel -lmenu
# Your .hpp files
DEPS = header.hpp #enums.cpp
# Your compiled .o files
OBJECTS = header.o #enums.o
# The name of your program
PROGRAM = WireMinnow
# The name of the Library
# ARCHIVE = TextWriter.a

.PHONY: all clean lint test

all: $(PROGRAM) #test $(ARCHIVE)

# Wildcard recipe to make .o files from corresponding .cpp file
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(ARCHIVE): $(OBJECTS)
	ar rcs $(ARCHIVE) $(OBJECTS)

test: test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o $(PROGRAM) $(ARCHIVE) test

lint:
	cpplint *.cpp *.hpp
