## Makefile config 

# Compilator
CC=g++

# Flags
CFLAGS=-c -Wall

# Sources
SOURCES=main.cpp QBchain.cpp QBcube.cpp QBHand.cpp qbmove_communications.cpp

# Executable
EXECUTABLE=main

# Objects
OBJECTS=$(SOURCES:.cpp=.o)

# Includes
INCLUDE=-I../include

# Linking Flags
LDFLAGS=-L../include
## ----------------------- 

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE):$(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(LDFLAGS) $(INCLUDE) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)
