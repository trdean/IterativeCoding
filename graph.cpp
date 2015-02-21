#include "graph.h"

Graph::Graph ( int dNumVariables, int dNumChecks, int **checkMatrix ) :
    numVariables( dNumVariables ),
    numChecks   ( dNumChecks )
{
    int i, j;

    variables.resize( numVariables );
    checks.resize( numChecks );

    for ( i = 0; i < numChecks; i++ ) {
        for ( j = 0; j < numVariables; j++ ) {
            if ( checkMatrix[i][j] == 1 ) {
                checks[i].PushReference( &variables[j] );
                variables[j].PushReference( &checks[i] );
            }
        }
    }      
}

void Graph::SetVariablesFromReal( double *values, double sigma )
{
    int i;

    for ( i = 0; i < numVariables; i++ ) {
        variables[i].SetSigma( sigma );
        variables[i].SetValueFromReal( values[i] );
    }
}

void Graph::SetVariablesFromLL( double *ll )
{
    int i;

    for ( i = 0; i < numVariables; i++ )
        variables[i].SetValueFromLL( ll[i] );
}

bool Graph::Decode( int maxIterations = MAX_ITER_DEFAULT )
{
    int i, j;
    bool updated;

    for( i = 0; i < maxIterations; i++ ) {
        updated = false;
        for ( j = 0; j < numChecks; j++ )
            updated |= checks[j].Update();
        for ( j = 0; j < numVariables; j++ )
            updated |= variables[j].Update();

        if (!updated)
            break;
    }

    return !updated;
}

void Graph::OutputHard( int *guess )
{
    int i;
    for ( i = 0; i < numVariables; i++ )
        guess[i] = variables[i].GetHardValue(); 
}
