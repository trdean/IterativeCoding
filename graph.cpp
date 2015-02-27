#include <stdio.h>
#include "graph.h"

Graph::Graph ( int dNumVariables, int dNumChecks, int **checkMatrix ) :
    numVariables( dNumVariables ),
    numChecks   ( dNumChecks )
{
    int i, j;

    AllocatedNodes = true;

    variables.resize( numVariables );
    checks.resize( numChecks );

    for ( i = 0; i < numChecks; i++ ) {
        checks[i] = new CheckNode();
        for ( j = 0; j < numVariables; j++ ) {
            variables[j] = new VariableNode();
            if ( checkMatrix[i][j] == 1 ) {
                checks[i]->PushReference( variables[j] );
                variables[j]->PushReference( checks[i] );
            }
        }
    }      
}

Graph::~Graph() 
{
    int i;

    if (AllocatedNodes) {
        for ( i = 0; i < numVariables; i++ ) 
            delete variables[i];
    }

    for ( i = 0; i < numChecks; i++ )
       delete checks[i]; 
}

Graph::Graph( const Graph& other )
{
    int i, j;

    numVariables = other.GetVariableLength();
    numChecks = other.GetCheckLength();
    AllocatedNodes = true;

    for ( i = 0; i < numChecks; i++ ) {
        checks[i] = new CheckNode();
        for ( j = 0; j < numVariables; j++ ) {
            variables[j] = new VariableNode();
            if ( other.CheckConnection( i, j ) ) {
                checks[i]->PushReference( variables[j] );
                variables[j]->PushReference( checks[i] );
            }
        }
    }
}

Graph& Graph::operator=( const Graph& rhs )
{
    int i, j;

    numVariables = rhs.GetVariableLength();
    numChecks = rhs.GetCheckLength();
    AllocatedNodes = true;

    for ( i = 0; i < numChecks; i++ ) {
        checks[i] = new CheckNode();
        for ( j = 0; j < numVariables; j++ ) {
            variables[j] = new VariableNode();
            if ( rhs.CheckConnection( i, j ) ) {
                checks[i]->PushReference( variables[j] );
                variables[j]->PushReference( checks[i] );
            }
        }
    }

    return *this;
}

bool Graph::CheckConnection( int checkIndex, int variableIndex ) const
{
    return variables[variableIndex]->IsReference( checks[checkIndex]->GetIndex() );
}

void Graph::SetVariablesFromReal( double *values, double sigma )
{
    int i;

    for ( i = 0; i < numVariables; i++ ) {
        variables[i]->SetSigma( sigma );
        variables[i]->SetValueFromReal( values[i] );
    }
}

void Graph::SetVariablesFromLL( double *ll )
{
    int i;

    for ( i = 0; i < numVariables; i++ )
        variables[i]->SetValueFromLL( ll[i] );
}

bool Graph::Decode( int maxIterations = MAX_ITER_DEFAULT )
{
    int i;

    for( i = 0; i < maxIterations; i++ ) {
        DecodeRound();
        if( CheckSyndrome() ) 
            return true;
    }

    return false;
}

void Graph::DecodeRound()
{
    int i;

    for ( i = 0; i < numChecks; i++ )
        checks[i]->Update();
    for ( i = 0; i < numVariables; i++ )
        variables[i]->Update();
}

void Graph::OutputHard( int *guess )
{
    int i;
    for ( i = 0; i < numVariables; i++ )
        guess[i] = variables[i]->GetHardValue(); 
}

int Graph::GetCheckLength() const
{
    return numChecks;
}

int Graph::GetVariableLength() const
{
    return numVariables;
}

bool Graph::CheckSyndrome()
{
    int i;

    for ( i = 0; i < numChecks; i++ )
        if (!checks[i]->Syndrome())
            return false;
    
    return true;
}

void Graph::Debug()
{
    int i;

    printf("-------------------------------------------\n");
    printf("Graph debug info\n");
    printf("----------------\n\n");

    for ( i = 0; i < numVariables; i++ )
        variables[i]->Debug();

    for ( i = 0; i < numChecks; i++ )
        checks[i]->Debug();

    printf("\n");
}
