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


    GenerateUnpermutedEdgeList();
    PermuteEdgeList();
    FixDoubleEdges();
    BuildGraph();    
}

Ldpc::Ldpc( int numberVariables,
            std::vector<double> lambda,
            std::vector<double> rho ) :
    Graph()
{
    unsigned i;
    std::vector<int> VariablesByDegree;
    std::vector<int> ChecksByDegree;

    srand(time(NULL));

    //Setup variable list
    numVariables = numberVariables;
    VariablesByDegree = LambdaToVariables( numVariables, lambda );
    BinToList( VariablesByDegree, &variableDegrees ); 
    
    for ( i = 0; i < variableDegrees.size(); i++ )
        numEdges += variableDegrees[i];

    //Setup check list
    ChecksByDegree = RhoToChecks( numEdges, rho );
    BinToList( ChecksByDegree, &checkDegrees );
    numChecks = checkDegrees.size();

    //Allocate nodes
    AllocatedNodes = true;
    for ( i = 0; i < numChecks; i++ )
        checks.push_back( new CheckNode() );
    for ( i = 0; i < numVariables; i++ )
        variables.push_back( new VariableNode() );

    GenerateUnpermutedEdgeList();
    PermuteEdgeList();
    FixDoubleEdges();
    BuildGraph();
}

Ldpc::~Ldpc()
{
    printf("Ldpc dtor\n");

    numEdges = 0;
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

    for( i = 0; i < numEdges; i++ ) {
        varIndex = EdgeListByVariable[i];
        checkIndex = GetCheckByEdgeIndex(i);
            
        //Now add the connections
        checks[checkIndex]->PushReference( variables[varIndex] );
        variables[varIndex]->PushReference( checks[checkIndex] );
    }
}

void Ldpc::FixDoubleEdges()
{
    int i, varIndex, checkIndex;

    for( i = 0; i < numEdges; i++ ) {
        varIndex = EdgeListByVariable[i];
        checkIndex = GetCheckByEdgeIndex(i);
            
        if( variables[varIndex]->IsReference( checks[checkIndex] ) )
              MoveEdge(i);
    
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
        if ( variables[badEdgeValue]->IsReference( checks[newCheck] ) )
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

/*
 * Use this to convert from the lists returned from LambdaToVariables
 * to the format used by variableDegrees and checkDegrees.  The intent
 * is that you pass the pointer to the member variable into this method
 *
 * ex:
 * Bins:
 * { 0, 2, 0, 3 }
 *
 * *result:
 * { 2, 2, 4, 4, 4 }
 * 
 */
void Ldpc::BinToList( std::vector<int> Bins, std::vector<int> *result )
{
   unsigned i;
   int j;
   
    for ( i = 0; i < Bins.size(); i++ ) {
        for ( j = 0; j < Bins[i]; j++ )
            result->push_back( i+1 );
    }
}

/*
 * Takes a polynomial in the edge perspective and returns a variable list
 * This is a binned list of degrees, stored like this:
 *
 * Index    :  0   1   2   3   4  ...
 * Degree   :  1   2   3   4   5  ... 
 * Num Nodes:  0   3   5   2   1  ...
 *
 */
std::vector<int> Ldpc::LambdaToVariables( int NumberVariables, 
                                          std::vector<double> EdgeNormal )
{
    unsigned i;
    int vars = 0;
    int temp;
    std::vector<int> Variables;

    //Convert perspective
    std::vector<double> L = EdgeNormalToNodeNormal( EdgeNormal );

    //Scale and round
    for ( i = 0; i < L.size(); i++ ) {
        temp = (int) ((NumberVariables*L[i]) + 0.5);
        vars += temp;
        Variables.push_back( temp );
    }

    //Adjust size to account for rounding.
    //Start at the back of the list and either add or remove nodes
    //until we have the right number
    while ( NumberVariables != vars ) {
        for ( i = Variables.size(); i > 0; i-- ) {
            if ( Variables[i] != 0 ) {
                if (NumberVariables > vars) {
                    Variables[i]++;
                    vars++;
                } else if (NumberVariables < vars) {
                    Variables[i]--;
                    vars--;
                }
            }
        }
    }

    return Variables;
}


/*
 * Converts a normalized edge-perspective distribution to a normalized node-
 * persepective distribution. 
 */
std::vector<double> Ldpc::EdgeNormalToNodeNormal( std::vector<double> EdgeNormal )
{
    unsigned i;
    double average_lambda;
    std::vector<double> lambda;

    //Find the average edges per node
    average_lambda = 0.0;
    for ( i = 0; i < EdgeNormal.size(); i++ )
        average_lambda += EdgeNormal[i]/(i+1);

    //Find the fraction of nodes by degree
    for ( i = 0; i < EdgeNormal.size(); i++ )
        lambda.push_back( EdgeNormal[i]/( average_lambda * (i+1) ) );

    return lambda;
}

/*
 * When we need to compute our check list, we already know how many edges
 * we have, so it's much easier to stay in the edge perspective.
 *
 * This outputs a list in the same format as LambdaToVariables.
 */
std::vector<int> Ldpc::RhoToChecks( int NumberEdges, 
                                    std::vector<double> EdgeNormal )
{
    unsigned i;
    std::vector<int> result;
    int ActualEdges;
    int temp;

    ActualEdges = 0;
    for( i = 0; i < EdgeNormal.size(); i++ ) {
        temp = (int) ((EdgeNormal[i] * NumberEdges / (i+1) ) + 0.5);
        result.push_back( temp );
        ActualEdges += temp * (i+1);
    }  

    //printf("Edges %d(%d)\n", NumberEdges, ActualEdges);
    //PrintVector( result );

    //Adjust to account for rounding
    //Start at the top, remove one from a bin and then
    //add one either to the next higher or next lower bin
    while( ActualEdges != NumberEdges ) {
        if ( ActualEdges > NumberEdges ) {
            for ( i = result.size(); i > 1; i-- ) {
                if ( result[i] != 0 ) {
                    result[i]--;
                    result[i-1]++;
                    ActualEdges--;
                }
            }
        } else {
             for ( i = result.size()-1; i > 0; i-- ) {
                if ( result[i] != 0 ) {
                    result[i]--;
                    result[i+1]++;
                    ActualEdges++;
                }
            }
        }
    }
    //PrintVector( result );
    return result;
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

void Ldpc::PrintVector( std::vector<int> vector ) {
    unsigned i;
    for ( i = 0; i < vector.size(); i++ ) {
        printf("%d ", vector[i]);
    }

    printf("\n");
}
