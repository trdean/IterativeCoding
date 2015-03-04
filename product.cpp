#include <stdlib.h>
#include <stdio.h>
#include "product.h"

Product::Product( Graph *graph1, Graph *graph2 )
{
    int x, y;

    graphVector.push_back( graph1 );
    graphVector.push_back( graph2 );
    
    x = graph1->GetVariableLength();
    y = graph2->GetVariableLength();
    
    dimensionWidth.push_back( y );
    dimensionWidth.push_back( x );

    strideVector.push_back( 1 );
    strideVector.push_back( y );

    dimensionLength.push_back( y );
    dimensionLength.push_back( 1 );

    dimension = 2;
    totalVariables = x*y;

    valueVector.resize( totalVariables );

    if ( y > x )
        maxWidth = y;
    else
        maxWidth = x;
}


bool Product::Decode( int maxIterations )
{
    int i;

    for ( i = 0; i < maxIterations; i++ ) {
        if( DecodeRound() )
            return true;
    }

    return false;
}

bool Product::DecodeRound()
{
    int i, j;
    bool syndrome = true;
    double *values = (double *) malloc( maxWidth * sizeof( double ) );

    for( i = 0; i < dimension; i++ ) {
        for( j = 0; j < dimensionWidth[i]; j++ ) {
            GetValues( i, j, values );
            graphVector[i]->SetVariablesFromLL( values );
            graphVector[i]->DecodeRound();
            graphVector[i]->GetValues( values );
            syndrome &= graphVector[i]->CheckSyndrome();
            SetValues( i, j, values );
        }
        if( syndrome )
            break;
    }    

    free( values );

    return syndrome;
}

void Product::GetValues( int dimension, int index, double *values )
{
    int i, width, length, stride;

    width  = dimensionWidth[dimension];
    length = dimensionLength[dimension];
    stride = strideVector[dimension];

    for( i = 0; i < width; i++ )
        values[i] = valueVector[index*length + i*stride];
}

void Product::SetValues( int dimension, int index, double *values )
{
    int i, width, length, stride;

    width  = dimensionWidth[dimension];
    length = dimensionLength[dimension];
    stride = strideVector[dimension];

    for( i = 0; i < width; i++ )
        valueVector[index*length + i*stride] = values[i];

}

void Product::SetVariablesFromReal( double *values, double sigma )
{
    int i;

    for ( i = 0; i < totalVariables; i++ ) 
        valueVector[i] = -2 * values[i] / (sigma * sigma);
}

void Product::SetVariablesFromLL( double *ll )
{
    int i;

    for ( i = 0; i < totalVariables; i++ )
        valueVector[i] = ll[i];
}

void Product::OutputHard( int *guess )
{
    int i;

    for ( i = 0; i < totalVariables; i++ ) {
        if ( valueVector[i] >= 0 )
            guess[i] = 0;
        else
            guess[i] = 1;
    }        
}

int Product::GetVariableLength()
{
    return totalVariables;
}

void Product::Debug()
{
    int i;

    printf("Variables:\n");

    for( i = 0; i < totalVariables; i++ ) {
        printf( "%f  ", valueVector[i] );
    }

    printf("\n\n");
}
