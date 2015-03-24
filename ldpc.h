#pragma once

#include <vector>
#include "graph.h"
#include "node.h"

class Ldpc : public Graph
{
    using Graph::Graph;

    public:
        Ldpc( FILE *checkMatrixFile ); //Import a graph from a file

        Ldpc( int numberVariables,  //Generate a regular graph 
              int variableDegree, 
              int checkDegree );

        Ldpc( int numberVariables,
              std::vector<double> lambda,
              std::vector<double> rho);

        ~Ldpc();

        void WriteGraph( FILE *checkMatrixFile );

        //Different stuff to handle irregular graphs?

        void PrintEdgeList();
        void PrintVector( std::vector<int> vector );

    private:
        int RandomInt( int max );
        void GenerateUnpermutedEdgeList();
        void PermuteEdgeList();
        void BuildGraph();

        void FixDoubleEdges();
        void MoveEdge( int index );
        int GetCheckByEdgeIndex( int index ); 

        void BinToList( std::vector<int> Bins, std::vector<int> *result );
        std::vector<double> EdgeNormalToNodeNormal( std::vector<double> EdgeNormal );
        std::vector<int> LambdaToVariables( int NumberVariables,
                                            std::vector<double> EdgeNormal );
        std::vector<int> RhoToChecks( int NumberEdges,
                                      std::vector<double> EdgeNormal );

        std::vector<int> variableDegrees;
        std::vector<int> checkDegrees;
        std::vector<int> EdgeListByVariable;

        int numEdges;
};
