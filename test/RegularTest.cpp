#include <stdio.h>
#include <vector>
#include "../CodeTest.h"
#include "../ldpc.h"

int main() {

    CodeTest *MyTest = new CodeTest();

    printf("(3,6) ensemble with 10 variables:\n");
    Ldpc *graph = new Ldpc( 10, 3, 6 );
    //graph->PrintEdgeList();
    MyTest->TestCode( graph,
                        0.85,
                        0.90,
                        0.01,
                        10,
                        1000,
                        stdout );
    //delete graph;

    printf("\n(3,6) ensemble with 100 variables:\n");
    Ldpc *graph2 = new Ldpc( 100, 3, 6 );
    //graph2->PrintEdgeList();
    MyTest->TestCode( graph2,
                        0.85,
                        0.90,
                        0.01,
                        10,
                        1000,
                        stdout );
    //delete graph2;

    printf("\n(3,6) ensemble with 1000 variables:\n");
    Ldpc *graph3 = new Ldpc( 1000, 3, 6 );
    //graph3->PrintEdgeList();
    MyTest->TestCode( graph3,
                        0.85,
                        0.90,
                        0.01,
                        10,
                        1000,
                        stdout );
    //delete graph;

    printf("\n(3, 6) ensemble with 10000 variables:\n");
    Ldpc *graph4 = new Ldpc( 10000, 3, 6 );
    MyTest->TestCode( graph4,
                        0.85,
                        0.90,
                        0.01,
                        5,
                        100,
                        stdout );

    printf("\n(3, 6) ensemble with 100000 variables:\n");
    Ldpc *graph5 = new Ldpc( 100000, 3, 6 );
    MyTest->TestCode( graph5,
                        0.85,
                        0.90,
                        0.01,
                        5,
                        100,
                        stdout );


    delete graph;
    delete graph2;
    delete graph3;
    delete graph4;
    delete graph5;
    delete MyTest;
}
