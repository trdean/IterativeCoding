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

        ~Ldpc();

        void WriteGraph( FILE *checkMatrixFile );

        //Different stuff to handle irregular graphs?

        void PrintEdgeList();

    private:
        int RandomInt( int max );
        void GenerateUnpermutedEdgeList();
        void PermuteEdgeList();
        void BuildGraph();
        void MoveEdge( int index );
        int GetCheckByEdgeIndex( int index ); 

        std::vector<int> variableDegrees;
        std::vector<int> checkDegrees;
        std::vector<int> EdgeListByVariable;
        bool FixDoubleEdges;

        int numEdges;
};
