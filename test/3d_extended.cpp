#include <stdio.h>

#include "../CodeTest.h"
#include "../graph.h"
#include "../product.h"

int main() 
{
    std::vector<std::vector<int> > hammingCheck;
    hammingCheck.push_back(std::vector<int> ({1, 1, 1, 0, 1, 0, 0, 0}));
    hammingCheck.push_back(std::vector<int> ({1, 1, 0, 1, 0, 1, 0, 0}));
    hammingCheck.push_back(std::vector<int> ({1, 0, 1, 1, 0, 0, 1, 0}));
    hammingCheck.push_back(std::vector<int> ({1, 1, 1, 1, 1, 1, 1, 1}));

    std::vector<std::vector<int> > parityCheck;
    parityCheck.push_back(std::vector<int> ({1, 1, 1}));

    Graph *HammingGraph = new Graph( &hammingCheck );
    Graph *ParityGraph = new Graph( &parityCheck );
    Product *HammingProduct = new Product( HammingGraph, HammingGraph );
    HammingProduct->Expand( ParityGraph );

    CodeTest *MyTest = new CodeTest();

    MyTest->TestProduct( HammingProduct,
                     0.0,
                     2.0,
                     0.05,
                       3,
                    10000,
                    stdout );  

    delete HammingGraph;
    delete HammingProduct;
    delete MyTest;
}
