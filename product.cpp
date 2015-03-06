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
    int i;
    unsigned j;
    std::vector<std::vector<int> > *productList;
    std::vector<int> *thisList;

    bool syndrome = true;
    double *values = (double *) malloc( maxWidth * sizeof( double ) );

    for( i = 0; i < dimension; i++ ) {
        productList = GenerateProduct( i );
        for( j = 0; j < productList->size(); j++ ) {
            //Reference to the jth vector from the list
            thisList = &(*productList)[j];
            GetValues( thisList, i, values );
            graphVector[i]->SetVariablesFromLL( values );
            graphVector[i]->DecodeRound();
            graphVector[i]->GetValues( values );
            syndrome &= graphVector[i]->CheckSyndrome();
            SetValues( thisList, i, values );
        }

        delete productList;
        if( syndrome )
            break;
    }    

    free( values );

    return syndrome;
}

void Product::GetValues( std::vector<int> *coordinates, 
                         int dimension, 
                         double *values )
{
    int i, index;

    for( i = 0; i < dimensionWidth[dimension]; i++ ) {
        (*coordinates)[dimension] = i;
        index = CoordinatesToIndex( coordinates );     
        values[i] = valueVector[index];
    }

    (*coordinates)[dimension] = 0;
}

void Product::SetValues( std::vector<int> *coordinates,
                         int dimension,
                         double *values )
{
    int i, index;

    for( i = 0; i < dimensionWidth[dimension]; i++ ) {
        (*coordinates)[dimension] = i;
        index = CoordinatesToIndex( coordinates );
        valueVector[index] = values[i];
    }

    (*coordinates)[dimension] = 0;
}

int Product::CoordinatesToIndex( std::vector<int> *coordinateVector )
{
    unsigned i;
    int index = 0;
    int stride = 1;

    for( i = 0; i < coordinateVector->size(); i++ ) {
        index += (*coordinateVector)[i]*stride;
        stride *= dimensionWidth[i];
    }

    return index;
}

std::vector<std::vector<int> > *Product::GenerateProduct( int dimToOmit )
{
    int i, j, numElements;
    std::vector<std::vector<int> > *result = 
        new std::vector<std::vector<int> >();

    std::vector<int> strides;
    strides.resize( dimension );
    int this_stride = 1;

    numElements = 1;
    for ( i = dimension - 1; i >= 0; i-- ) {
        if ( i != dimToOmit ) {
            numElements *= dimensionWidth[i];
            strides[i] = this_stride;
            this_stride *= dimensionWidth[i];
        }
    }

    result->resize( numElements );

    for ( i = 0; i < numElements; i++ ) {
        (*result)[i].resize( dimension );
        for ( j = 0; j < dimension; j++ ) {
            if ( j != dimToOmit )
                (*result)[i][j] = (i/strides[j]) % dimensionWidth[j];
            else
                (*result)[i][j] = 0;
        }
    }

    return result;
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
        if ( (i+1) % dimensionWidth[0] == 0 )
            printf( "\n" );
    }

    printf("\n\n");
}

void Product::DebugVector( std::vector<std::vector<int> > *vector )
{
    unsigned i, j;
    for ( i = 0; i < vector->size(); i++ ) {
        for ( j = 0; j < (*vector)[i].size(); j++ ) {
            printf("%d ", (*vector)[i][j]);
        }
        printf("\n");
    }
}
