#include <stdio.h>

#include "../CodeTest.h"
#include "../graph.h"

int main() 
{
    std::vector<std::vector<int> > hammingCheck;
    hammingCheck.push_back(std::vector<int> ({1, 1, 1, 0, 1, 0, 0}));
    hammingCheck.push_back(std::vector<int> ({1, 1, 0, 1, 0, 1, 0}));
    hammingCheck.push_back(std::vector<int> ({1, 0, 1, 1, 0, 0, 1}));

    Graph *HammingGraph = new Graph( &hammingCheck );
    Graph Copy = Graph( *HammingGraph );
    Graph Assign = *HammingGraph;

    CodeTest *MyTest = new CodeTest();

    printf( "Original\n\n" );

    MyTest->TestCode( HammingGraph,
                     0.0,
                     2.0,
                     0.05,
                      10,
                    10000,
                    stdout );  

    printf( "\n\nCopied\n\n" );

    MyTest->TestCode( &Copy,
                        0.0,
                        2.0,
                       0.05,
                         10,
                      10000,
                     stdout );

    printf( "\n\nAssigned\n\n" );

    MyTest->TestCode( &Assign,
                        0.0,
                        2.0,
                       0.05,
                         10,
                      10000,
                     stdout );

    delete HammingGraph;
    delete MyTest;
}
