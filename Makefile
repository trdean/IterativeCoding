CC = g++
CFLAGS = -Wall -g -std=c++11
LIBS = -lm -lstdc++

default : hamming 2d_hamming parity bp_debug copy_test 2d_debug

hamming : test/hamming_test.cpp node.o graph.o product.o CodeTest.o
	$(CC) $(CFLAGS) -o test/hamming_test test/hamming_test.cpp node.cpp graph.cpp product.cpp CodeTest.cpp

2d_hamming : test/2d_hamming_test.cpp node.o graph.o product.o CodeTest.o
	$(CC) $(CFLAGS) -o test/2d_hamming_test test/2d_hamming_test.cpp node.cpp graph.cpp product.cpp CodeTest.cpp 

parity : test/parity_test.cpp node.o graph.o 
	$(CC) $(CFLAGS) -o test/parity_test test/parity_test.cpp node.cpp graph.cpp 

bp_debug : test/bp_debug.cpp node.o graph.o CodeTest.o
	$(CC) $(CFLAGS) -o test/bp_debug test/bp_debug.cpp node.cpp graph.cpp product.cpp CodeTest.cpp

2d_debug : test/2d_debug.cpp node.o graph.o product.o CodeTest.o
	$(CC) $(CFLAGS) -o test/2d_debug test/2d_debug.cpp node.cpp graph.cpp product.cpp CodeTest.cpp

copy_test : test/copy_test.cpp node.o graph.o CodeTest.o
	$(CC) $(CFLAGS) -o test/copy_test test/copy_test.cpp node.cpp graph.cpp product.cpp CodeTest.cpp

node.o : node.cpp node.h
	$(CC) $(CFLAGS) -c node.cpp $(LIBS)

graph.o: graph.cpp graph.h
	$(CC) $(CFLAGS) -c graph.cpp $(LIBS)

product.o : product.cpp product.h
	$(CC) $(CFLAGS) -c product.cpp $(LIBS)

CodeTest.o: CodeTest.cpp CodeTest.h
	$(CC) $(CFLAGS) -c CodeTest.cpp $(LIBS)

doc:
	doxygen doxygen.cfg

clean:
	rm *.o test/parity_test test/hamming_test test/bp_debug test/copy_test test/*.o
