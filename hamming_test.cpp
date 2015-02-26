#include <stdio.h>

#include "CodeTest.h"
#include "graph.h"

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

    Graph *HammingGraph = new Graph( 7, 3, mCheck );

    CodeTest *MyTest = new CodeTest();

    MyTest->TestCode( HammingGraph,
                     0.0,
                     2.0,
                     0.05,
                      10,
                    10000,
                    stdout );  

    for (i = 0; i < 3; i++ )
        free( mCheck[i] );
    free( mCheck );
}
