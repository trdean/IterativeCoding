#include <stdio.h>
#include <vector>
#include "../CodeTest.h"
#include "../ldpc.h"

int main() {

    CodeTest *MyTest = new CodeTest();

    printf("(3,6) ensemble with 10 variables:\n");
    Ldpc *graph = new Ldpc( 10, 3, 6 );
    graph->PrintEdgeList();
    MyTest->TestCode( graph,
                        0.0,
                        1.0,
                        0.05,
                        10,
                        10000,
                        stdout );
    //delete graph;

    printf("\n(3,6) ensemble with 9 variables:\n");
    Ldpc *graph2 = new Ldpc( 9, 3, 6 );
    graph2->PrintEdgeList();
    MyTest->TestCode( graph2,
                        0.0,
                        1.0,
                        0.05,
                        10,
                        10000,
                        stdout );
    //delete graph2;

    printf("\n(3,8) ensemble with 21 variables:\n");
    Ldpc *graph3 = new Ldpc( 21, 3, 8 );
    graph3->PrintEdgeList();
    MyTest->TestCode( graph3,
                        0.0,
                        1.0,
                        0.05,
                        10,
                        10000,
                        stdout );
    //delete graph;

    printf("\n(12, 10) ensemble with 5000 variables:\n");
    Ldpc *graph4 = new Ldpc( 5000, 12, 10 );
    MyTest->TestCode( graph4,
                        0.0,
                        1.0,
                        0.05,
                        5,
                        200,
                        stdout );

    delete graph;
    delete graph2;
    delete graph3;
    delete graph4;
    delete MyTest;
}
