#pragma once

#include <vector>
#include "node.h"

#define MAX_ITER_DEFAULT 30

class Graph
{
    public:
        Graph( int dNumVariables, int dNumChecks, int **checkMatrix );
        Graph( std::vector<VariableNode *> *variableNodes, int dNumChecks, int **checkMatrix );
        ~Graph();
        Graph( const Graph& other );
        Graph& operator=( const Graph& rhs );

        bool CheckConnection( int checkIndex, int variableIndex ) const;

        void SetVariablesFromReal( double *values, double sigma );
        void SetVariablesFromLL( double *ll );
        bool Decode( int maxIterations );
        void DecodeRound();
        void OutputHard( int *guess );
        int GetCheckLength() const;
        int GetVariableLength() const;
        bool CheckSyndrome();

        void Debug();

    private:
        std::vector<VariableNode *>   variables;
        std::vector<CheckNode *>      checks;

        int numVariables;
        int numChecks;
        bool AllocatedNodes;
};
