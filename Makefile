CC = g++
CFLAGS = -Wall
LIBS = -lm -lstdc++

default : node.o graph.o
	$(CC) $(CFLAGS) -o libIter node.o graph.o

node.o : node.cpp node.h
	$(CC) $(CFLAGS) -c node.cpp $(LIBS)

graph.o: graph.cpp graph.h
	$(CC) $(CFLAGS) -c graph.cpp $(LIBS)
