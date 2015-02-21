#include "graph.h"

Graph::Graph ( int dNumVariables, int dNumChecks, int **checkMatrix ) :
    numVariables( dNumVariables ),
    numChecks   ( dNumChecks )
{
    int i, j;

    variables = new VariableNode[ numVariables ];
    checks = new CheckNode[ numChecks ];

    for ( i = 0; i < ??; i++ ) {
        for ( j = 0; j < ??; j++ ) {
            if ( checkMatrix[i][j] == 1 ) {
            }
        }
    }      
}
