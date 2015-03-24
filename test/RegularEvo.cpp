#include <stdio.h>
#include <vector>
#include "../CodeTest.h"
#include "../ldpc.h"

int main() {

    CodeTest *MyTest = new CodeTest();

    printf("(3,6) ensemble with 100,000 variables:\n");
    Ldpc *graph = new Ldpc( 100000, 3, 6 );
    
    while (1)
        MyTest->Evolution( graph,
                            0.80,
                            1000 );
    delete graph;
    delete MyTest;
}
