#pragma once

#include <vector>
#include "node.h"
#include "graph.h"

class Product
{
    public:
        Product( Graph *graph1, Graph *graph2 );
        ~Product();
        //Copy
        //Assignment

        bool Decode( int maxIterations );
        void DecodeRound();
        bool CheckSyndrome();

        void SetVariablesFromReal( double *values, double sigma );
        void SetVariablesFromLL( double *ll );
        void OutputHard( int *guess );
        int GetWidth();
        int GetHeight();
        int GetVariableLength();

        void Debug();

    private:
        std::vector<Graph *> graphDim1;
        std::vector<Graph *> graphDim2;

        int lenDim1;
        int lenDim2;
};
