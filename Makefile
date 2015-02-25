CC = g++
CFLAGS = -Wall -g -std=c++11
LIBS = -lm -lstdc++

default : node.o graph.o CodeTest.o
	$(CC) $(CFLAGS) -o libIter node.o graph.o CodeTest.o

hamming : hamming_test.cpp node.o graph.o CodeTest.o
	$(CC) $(CFLAGS) -o hamming_test hamming_test.cpp node.cpp graph.cpp CodeTest.cpp

node.o : node.cpp node.h
	$(CC) $(CFLAGS) -c node.cpp $(LIBS)

graph.o: graph.cpp graph.h
	$(CC) $(CFLAGS) -c graph.cpp $(LIBS)

CodeTest.o: CodeTest.cpp CodeTest.h
	$(CC) $(CFLAGS) -c CodeTest.cpp $(LIBS)
