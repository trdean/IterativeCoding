#include <stdio.h>
#include <vector>
#include <exception>
#include "graph.h"

/* Old constructor, now deprecated
Graph::Graph ( int dNumVariables, int dNumChecks, int **checkMatrix ) :
    numVariables( dNumVariables ),
    numChecks   ( dNumChecks )
{
    unsigned i, j;

    AllocatedNodes = true;

    variables.resize( numVariables );
    checks.resize( numChecks );

    for ( i = 0; i < numChecks; i++ )
        checks[i] = new CheckNode();
    for ( i = 0; i < numVariables; i++ )
        variables[i] = new VariableNode();

    for ( i = 0; i < numChecks; i++ ) {
        for ( j = 0; j < numVariables; j++ ) {
            if ( checkMatrix[i][j] == 1 ) {
                checks[i]->PushReference( variables[j] );
                variables[j]->PushReference( checks[i] );
            }
        }
    }
}
*/

Graph::Graph() : 
    LinearCode()
{
    numVariables = 0;
    numChecks = 0;
    AllocatedNodes = false;
    sigma = 0.0;
}

Graph::Graph( std::vector<std::vector<int> > *checkMatrix ) :
    LinearCode()
{
    unsigned i, j;

    numChecks = checkMatrix->size();
    numVariables = (*checkMatrix)[0].size();

    AllocatedNodes = true;

    variables.resize( numVariables );
    checks.resize( numChecks );

    for ( i = 0; i < numChecks; i++ )
        checks[i] = new CheckNode();
    for ( i = 0; i < numVariables; i++ )
        variables[i] = new VariableNode();

    for ( i = 0; i < numChecks; i++ ) {
        if ( (*checkMatrix)[i].size() != numVariables ) {
            for ( j = 0; j < numChecks; j++ )
                delete checks[i];
            for ( j = 0; j < numVariables; j++ )
                delete variables[i];

            throw;
        }
        for ( j = 0; j < numVariables; j++ ) {
            if ( (*checkMatrix)[i][j] == 1 ) {
                checks[i]->PushReference( variables[j] );
                variables[j]->PushReference( checks[i] );
            }
        }
    }
}

Graph::Graph( Graph *other, std::vector<VariableNode *> *variableNodes ) :
    LinearCode()
{
    unsigned i, j;

    numVariables = other->GetVariableLength();
    numChecks = other->GetCheckLength();
    AllocatedNodes = false;

    variables.resize( numVariables );
    checks.resize( numChecks );

    for ( i = 0; i < numChecks; i++ )
        checks[i] = new CheckNode();
    for ( i = 0; i < numVariables; i++ )
        variables[i] = (*variableNodes)[i];

    for ( i = 0; i < numChecks; i++ ) {
        for ( j = 0; j < numVariables; j++ ) {
            if ( other->CheckConnection( i, j ) ) {
                checks[i]->PushReference( variables[j] );
                variables[j]->PushReference( checks[i] );
            }
        }
    }
}

Graph::~Graph() 
{
    unsigned i;

    if (AllocatedNodes) {
        for ( i = 0; i < numVariables; i++ ) 
            delete variables[i];
    }

    for ( i = 0; i < numChecks; i++ )
       delete checks[i]; 
}

Graph::Graph( const Graph& other )
{
    unsigned i, j;

    numVariables = other.GetVariableLength();
    numChecks = other.GetCheckLength();
    AllocatedNodes = true;

    variables.resize( numVariables );
    checks.resize( numChecks );

    for ( i = 0; i < numChecks; i++ )
        checks[i] = new CheckNode();
    for ( i = 0; i < numVariables; i++ )
        variables[i] = new VariableNode();

    for ( i = 0; i < numChecks; i++ ) {
        for ( j = 0; j < numVariables; j++ ) {
            if ( other.CheckConnection( i, j ) ) {
                checks[i]->PushReference( variables[j] );
                variables[j]->PushReference( checks[i] );
            }
        }
    }
}

Graph& Graph::operator=( const Graph& rhs )
{
    unsigned i, j;

    numVariables = rhs.GetVariableLength();
    numChecks = rhs.GetCheckLength();
    AllocatedNodes = true;

    checks.resize( numChecks );
    variables.resize( numVariables );

    for ( i = 0; i < numChecks; i++ )
        checks[i] = new CheckNode();
    for ( i = 0; i < numVariables; i++ )
        variables[i] = new VariableNode();

    for ( i = 0; i < numChecks; i++ ) {
        for ( j = 0; j < numVariables; j++ ) {
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

void Graph::SetVariablesFromReal( double *values )
{
    SetVariablesFromReal( values, variables[0]->GetSigma() );
}

void Graph::SetVariablesFromReal( double *values, double sigma )
{
    unsigned i;

    for ( i = 0; i < numVariables; i++ ) {
        variables[i]->SetSigma( sigma );
        variables[i]->SetValueFromReal( values[i] );
    }
}

void Graph::SetVariablesFromLL( double *ll )
{
    unsigned i;

    for ( i = 0; i < numVariables; i++ )
        variables[i]->SetValueFromLL( ll[i] );
}

void Graph::GetValues( double *values )
{
    unsigned i;

    for ( i = 0; i < numVariables; i++ )
        values[i] = variables[i]->value;
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
    unsigned i;

    for ( i = 0; i < numChecks; i++ )
        checks[i]->Update();
    for ( i = 0; i < numVariables; i++ )
        variables[i]->Update();
}

void Graph::OutputHard( int *guess )
{
    unsigned i;

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
    unsigned i;

    for ( i = 0; i < numChecks; i++ )
        if (!checks[i]->Syndrome())
            return false;
    
    return true;
}

VariableNode *Graph::GetVariable( int index )
{
    return variables[index];
}

CheckNode *Graph::GetCheck( int index )
{
    return checks[index];
}

void Graph::Debug()
{
    unsigned i;

    printf("-------------------------------------------\n");
    printf("Graph debug info\n");
    printf("----------------\n\n");

    for ( i = 0; i < numVariables; i++ )
        variables[i]->Debug();

    for ( i = 0; i < numChecks; i++ )
        checks[i]->Debug();

    printf("\n");
}

void Graph::ViewGraph()
{
    unsigned i;
    int j;

    printf("Graph connections\n\n");

    for ( i = 0; i < numVariables; i++ ) {
        printf("Variable Node %d:\n", variables[i]->GetIndex() );
        for ( j = 0; j < variables[i]->GetDegree(); j++ ) 
            printf("    Check Node %d\n", variables[i]->GetReference( j )->GetIndex() );
    }

    for ( i = 0; i < numChecks; i++ ) {
        printf("Check Node %d:\n", checks[i]->GetIndex() );
        for ( j = 0; j < checks[i]->GetDegree(); j++ )
            printf("    Variable Node %d\n", checks[i]->GetReference( j )->GetIndex() );
    }
 
}
