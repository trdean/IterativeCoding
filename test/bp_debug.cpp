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

    CodeTest *MyTest = new CodeTest();

    MyTest->TestDebug( HammingGraph,
                       0.5,
                       2 );
/*
    MyTest->TestCode( HammingGraph,
                     0.0,
                     2.0,
                     0.05,
                      10,
                    10000,
                    stdout );  
*/

    delete HammingGraph;
    delete MyTest;
}
