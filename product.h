#pragma once

#include <vector>
#include "node.h"
#include "graph.h"

class Product
{
    public:
        Product( Graph *graph1, Graph *graph2 );
        //~Product();
        //Copy
        //Assignment

        void Expand( Graph *graph );

        bool Decode( int maxIterations );
        bool DecodeRound();

        void GetValues( std::vector<int> *coordinates, int dimension, double *values );
        void SetValues( std::vector<int> *coordinates, int dimension, double *values );
        int CoordinatesToIndex( std::vector<int> *coordinate );
        std::vector<std::vector<int> > *GenerateProduct( int dimToOmit );

        void SetVariablesFromReal( double *values, double sigma );
        void SetVariablesFromLL( double *ll );
        void OutputHard( int *guess );
        int GetVariableLength();

        void Debug();
        void DebugVector( std::vector<std::vector<int> > *vector );

    private:
        int dimension;
        int totalVariables;
        int maxWidth;

        std::vector<Graph *> graphVector;
        std::vector<int> dimensionWidth;
        std::vector<double> valueVector;
};
