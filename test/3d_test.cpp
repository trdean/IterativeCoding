#include <stdio.h>

#include "../CodeTest.h"
#include "../graph.h"
#include "../product.h"

int main() 
{
    int hammingCheck[][7] = { { 1, 1, 1, 0, 1, 0, 0},
             			      { 1, 1, 0, 1, 0, 1, 0},
	            		      { 1, 0, 1, 1, 0, 0, 1} };

    int **mCheck = (int **) malloc(3*sizeof(int *));
    int i,j;
    for ( i = 0; i < 3; i++ ) {
        mCheck[i] = (int *) malloc(7*sizeof(int *));
        for ( j = 0; j < 7; j++ ) {
            mCheck[i][j] = hammingCheck[i][j];
        }
    }

    int **parity = (int **) malloc(sizeof(int *));
    parity[0] = (int *) malloc(3*sizeof(int));
    parity[0][0] = 1; parity[0][1] = 1; parity[0][2] = 1;
    

    Graph *HammingGraph = new Graph( 7, 3, mCheck );
    Graph *ParityGraph = new Graph( 3, 1, (int **) &parity );
    Product *HammingProduct = new Product( HammingGraph, HammingGraph );
    HammingProduct->Expand( ParityGraph );

    CodeTest *MyTest = new CodeTest();

    MyTest->TestProduct( HammingProduct,
                     0.0,
                     2.0,
                     0.05,
                      10,
                    10000,
                    stdout );  

    for (i = 0; i < 3; i++ )
        free( mCheck[i] );
    free( mCheck );
    free( parity[0] ); free( parity );

    delete HammingGraph;
    delete HammingProduct;
    delete MyTest;
}
