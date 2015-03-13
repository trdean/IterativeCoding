#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "ldpc.h"
#include "graph.h"
#include "node.h" 

Ldpc::Ldpc( FILE *checkMatrixFile ) :
    Graph ()
{
    srand(time(NULL));
}

Ldpc::Ldpc( int numberVariables,
            int variableDegree,
            int checkDegree ) :
    Graph ()
{
    unsigned i;
    int edgeRemainder;
    srand(time(NULL));

    numVariables = numberVariables;
    numEdges = numVariables * variableDegree;
    numChecks = numEdges / checkDegree;
    edgeRemainder = numEdges % checkDegree;

    AllocatedNodes = true;
    for ( i = 0; i < numChecks; i++ )
        checks.push_back( new CheckNode() );
    for ( i = 0; i < numVariables; i++ )
        variables.push_back( new VariableNode() );

    //Setup list with degrees of check and variable nodes
    for ( i = 0; i < numVariables; i++ )
        variableDegrees.push_back( variableDegree );

    for ( i = 0; i < numChecks; i++ )
        checkDegrees.push_back( checkDegree );

    //Account for left of edges if numVars does not divide both degrees
    for ( i = numChecks - edgeRemainder; i < numChecks; i++ )
        checkDegrees[i]++;

    FixDoubleEdges = true;

    GenerateUnpermutedEdgeList();
    PermuteEdgeList();
    BuildGraph();    
}

Ldpc::~Ldpc()
{
/*
    unsigned i;

    if( AllocatedNodes ) {
        for ( i = 0; i < numVariables; i++ )
            delete variables[i];
    }

    for ( i = 0; i < numChecks; i++ )
        delete checks[i];
*/    
}

void Ldpc::WriteGraph( FILE *checkMatrixFile )
{

}

int Ldpc::RandomInt( int max )
{
    double uniform = ( (double) rand() ) * ( 1.0 / (double) RAND_MAX );
    return (int) ( uniform * (double) ( max - 1 ) );
}

/*
 * This sets up the vector EdgeListByVariable, which is a list of which
 * variable node each edge points to.  The check node they point it is
 * determined by its index in the array.  This makes an unpermuted edge
 * list which we then pass to a separate function to make a random graph
 *
 * For example if we have 4 variable nodes of degree 3 and 3 check nodes 
 * of degree 4, we'd get:
 *
 * {0, 0, 0, 1,|1, 1, 2, 2,|2, 3, 3, 3 }
 * +--check 0--+--check 1--+--check 2--+
 *
 * Which we then need to pass through a random permutation and fix double
 * edges, and optionally do things like look for short cycles.
 */
void Ldpc::GenerateUnpermutedEdgeList()
{
    unsigned i;
    int j;

    EdgeListByVariable.clear();

    for ( i = 0; i < numVariables; i++ ) {
        for ( j = 0; j < variableDegrees[i]; j++ ) {
            EdgeListByVariable.push_back( i );
        }
    }
}

void Ldpc::PermuteEdgeList()
{
    unsigned i;
    int random, temp;

    for( i = 0; i < EdgeListByVariable.size(); i++ ) {
        random = RandomInt( EdgeListByVariable.size() );
        temp = EdgeListByVariable[i];
        EdgeListByVariable[i] = EdgeListByVariable[random];
        EdgeListByVariable[random] = temp;
    }
}

void Ldpc::BuildGraph()
{
    int i, varIndex, checkIndex;
    int deduped = 0;

    for( i = 0; i < numEdges; i++ ) {
        //Becomes true if we are not checking doubles or if we see that it is not
        //a double
        while( deduped != 2 ) {
            varIndex = EdgeListByVariable[i];
            checkIndex = GetCheckByEdgeIndex(i);
            
            if( FixDoubleEdges && deduped != 1 ) {
                //Check to see if we have already added this edge
                if( variables[varIndex]->
                        IsReference( checks[checkIndex]->GetIndex() ) ) {
                    MoveEdge(i);
                    deduped = 1;
                } else deduped = 2;
            } else deduped = 2;
        }
        deduped = 0;

        //Now add the connections
        checks[checkIndex]->PushReference( variables[varIndex] );
        variables[varIndex]->PushReference( checks[checkIndex] );
    }
}

/*
 * Fix a double edge by permuting it to a random location on the graph.
 * And check that you don't make a new double edge in the process.
 * 
 * If we can't fix it, we just leave it in place.  When we add references
 * to our graph, we skip edges that are double edges.
 */ 
void Ldpc::MoveEdge( int index )
{
    int badEdgeValue, newEdgeValue, newIndex;
    int temp, i, newCheck;

    for( i = 0; i < 5; i++ ) {
        //Get the old and new variable nodes
        badEdgeValue = EdgeListByVariable[index];
        newIndex = RandomInt( numEdges );
        newEdgeValue = EdgeListByVariable[newIndex];
    
        //We are going to swap the two we need to check whether or not each
        //move is going to create a new double edge (two checks).  We can 
        //loop until we hit both conditions.  We'll stop trying after a couple
        //loops so this doesn't take too long.  If we fail we'll just remove 
        //the edge.
    
        //First check if the new variable node is the same as the old one
        if ( newEdgeValue == badEdgeValue ) 
            continue;

        //The check if moving the old variable node to a new check node will
        //create a double edge there.
        newCheck = GetCheckByEdgeIndex( newIndex );
        if ( variables[badEdgeValue]->IsReference( checks[newCheck]->GetIndex() ) )
            continue;

        //Perform the swap
        temp = badEdgeValue;
        EdgeListByVariable[index] = newEdgeValue;
        EdgeListByVariable[newIndex] = temp;

        //If we got this far we don't have a double edge
        break;
    }
}

int Ldpc::GetCheckByEdgeIndex( int index )
{
    unsigned i;
 
    for ( i = 0; i < numChecks; i++ ) {
        index -= checkDegrees[i];
        if ( index < 0 )
            return i;
    }

    return i;
}

void Ldpc::PrintEdgeList()
{
    unsigned i;
    int j;

    for ( i = 0; i < numChecks; i++ )
        for ( j = 0; j < checkDegrees[i]; j++ )
            printf("%02d ", i);
    printf("\n");

    for ( i = 0; i < (unsigned) numEdges; i++ ) 
        printf("%02d ", EdgeListByVariable[i]);
    printf("\n");
}
