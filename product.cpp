#include <stdio.h>
#include "product.h"

Product::Product( Graph *graph1, Graph *graph2 )
{
    int i, j;
    std::vector<VariableNode *> tmpVarVec;

    lenDim1 = graph1->GetVariableLength();
    lenDim2 = graph2->GetVariableLength();

    //Make lenDim2 copies of graph1 and store them in a vector
    for ( i = 0; i < lenDim2; i++ ) {
        graphDim1.push_back( new Graph( *graph1 ) );
    }

    for ( i = 0; i < lenDim1; i++ ) {
        for ( j = 0; j < lenDim2; j++ )
            tmpVarVec.push_back( graphDim1[j]->GetVariable( i ) );
        
        graphDim2.push_back( new Graph( graph2, &tmpVarVec ) );
        tmpVarVec.clear();
    }
}

Product::~Product()
{
    int i;

    for ( i = 0; i < lenDim2; i++ )
        delete graphDim1[i];

    for ( i = 0; i < lenDim1; i++ )
        delete graphDim2[i];
}

bool Product::Decode( int maxIterations )
{
    int i;

    for ( i = 0; i < maxIterations; i++ ) {
        DecodeRound();
        if( CheckSyndrome() )
            return true;
    }

    return false;
}

void Product::DecodeRound()
{
    int i;

    for ( i = 0; i < lenDim2; i++ )
        graphDim1[i]->DecodeRound();

    for ( i = 0; i < lenDim1; i++ )
        graphDim2[i]->DecodeRound();
}

bool Product::CheckSyndrome()
{
    int i;
    
    for ( i = 0; i < lenDim2; i++ )
        if (!graphDim1[i]->CheckSyndrome())
            return false;

    for ( i = 0; i < lenDim1; i++ )
        if (!graphDim2[i]->CheckSyndrome())
            return false;

    return true;
}

void Product::SetVariablesFromReal( double *values, double sigma )
{
    int i;

    for ( i = 0; i < lenDim2; i++ ) {
        graphDim1[i]->SetVariablesFromReal( &values[i*lenDim1], sigma );
    }
}

void Product::SetVariablesFromLL( double *ll )
{
    int i;

    for ( i = 0; i < lenDim2; i++ ) {
        graphDim1[i]->SetVariablesFromLL( &ll[i*lenDim1] );
    }
}

void Product::OutputHard( int *guess )
{
    int i, j;
    for ( i = 0; i < lenDim2; i++ )
        for ( j = 0; j < lenDim1; j++ )
            guess[i*lenDim2 + j] = graphDim1[i]->GetVariable( j )->GetHardValue();
}

int Product::GetWidth()
{
    return lenDim2;
}

int Product::GetHeight()
{
    return lenDim1;
}

int Product::GetVariableLength()
{
    return lenDim1*lenDim2;
}

void Product::Debug()
{
    int i, j;

    printf("Variables:\n");

    for( i = 0; i < lenDim2; i++ ) {
        for ( j = 0; j < lenDim1; j++ ) {
            printf( "%f  ", graphDim1[i]->GetVariable(j)->value );
        }
        printf("\n");
    }

    printf("\n\n");
}
