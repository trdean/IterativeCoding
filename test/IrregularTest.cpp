#include <stdio.h>
#include <vector>
#include "../CodeTest.h"
#include "../ldpc.h"

int main() {

    CodeTest *MyTest = new CodeTest();

    printf("RSU ensemble, 10 vars:\n");
    std::vector<double> RSULambda;
    RSULambda.push_back( 0.0 );
    RSULambda.push_back( 0.21991 );
    RSULambda.push_back( 0.23328 );
    RSULambda.push_back( 0.02058 );
    RSULambda.push_back( 0.08543 );
    RSULambda.push_back( 0.06540 );
    RSULambda.push_back( 0.04767 );
    RSULambda.push_back( 0.01912 );

    std::vector<double> RSURho;
    RSURho.push_back( 0.0 );
    RSURho.push_back( 0.0 );
    RSURho.push_back( 0.0 );
    RSURho.push_back( 0.0 );
    RSURho.push_back( 0.0 );
    RSURho.push_back( 0.0 );
    RSURho.push_back( 0.64854 ); 
    RSURho.push_back( 0.34747 );
    RSURho.push_back( 0.00399 );

    Ldpc *graph = new Ldpc( 100, RSULambda, RSURho );
    //graph->PrintEdgeList();
    //graph->ViewGraph();
    MyTest->TestCode( graph,
                        0.0,
                        1.0,
                        0.05,
                        6,
                        1000,
                        stdout );
    delete graph;    
    
    printf("\nRSU ensemble, 20 vars\n");
    graph = new Ldpc( 20, RSULambda, RSURho );
    graph->PrintEdgeList();
    graph->ViewGraph();
    MyTest->TestCode( graph,
                        0.0,
                        1.0,
                        0.05,
                        10,
                        10000,
                        stdout );
    delete graph;

    
    printf("\nRSU ensemble with 100 variables:\n");
    graph = new Ldpc( 100, RSULambda, RSURho );
    //graph3->PrintEdgeList();
    MyTest->TestCode( graph,
                        0.0,
                        1.0,
                        0.05,
                        10,
                        1000,
                        stdout );
    delete graph;
    
    printf("\n(12, 10) ensemble with 5000 variables:\n");
    graph = new Ldpc( 5000, RSULambda, RSURho );
    MyTest->TestCode( graph,
                        0.0,
                        1.0,
                        0.05,
                        5,
                        200,
                        stdout );
    
    delete graph;
    //delete graph2;
    //delete graph3;
    //delete graph4;
    delete MyTest;
}
