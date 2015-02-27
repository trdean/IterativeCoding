#include <stdio.h>
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
    int i;
    bool success;

    success = false;
    for( i = 0; i < maxIterations; i++ ) {
        if (!DecodeRound()) {
            success = true;
            break;
        }
    }

    return success;
}

bool Graph::DecodeRound()
{
    int i;
    bool updated;

    updated = false;
    for ( i = 0; i < numChecks; i++ )
        updated |= checks[i].Update();
    for ( i = 0; i < numVariables; i++ )
        updated |= variables[i].Update();

    return updated; 
}

void Graph::OutputHard( int *guess )
{
    int i;
    for ( i = 0; i < numVariables; i++ )
        guess[i] = variables[i].GetHardValue(); 
}

int Graph::GetCheckLength()
{
    return numChecks;
}

int Graph::GetVariableLength()
{
    return numVariables;
}

void Graph::Debug()
{
    int i;

    printf("-------------------------------------------\n");
    printf("Graph debug info\n");
    printf("----------------\n\n");

    for ( i = 0; i < numVariables; i++ )
        variables[i].Debug();

    for ( i = 0; i < numChecks; i++ )
        checks[i].Debug();

    printf("\n");
}
