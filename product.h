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

        bool Decode( int maxIterations );
        bool DecodeRound();

        void GetValues( int dimension, int index, double *values );
        void SetValues( int dimension, int index, double *values );

        void SetVariablesFromReal( double *values, double sigma );
        void SetVariablesFromLL( double *ll );
        void OutputHard( int *guess );
        int GetVariableLength();

        void Debug();

    private:
        int dimension;
        int totalVariables;
        int maxWidth;

        std::vector<Graph *> graphVector;
        std::vector<int> dimensionWidth;
        std::vector<int> dimensionLength;
        std::vector<int> strideVector;
        std::vector<double> valueVector;
};
