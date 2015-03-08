#pragma once

#include <vector>
#include "linearcode.h"
#include "node.h"

#define MAX_ITER_DEFAULT 30

class Graph : public LinearCode
{
    public:
        //Graph( int dNumVariables, int dNumChecks, int **checkMatrix );
        Graph( std::vector<std::vector<int> > *checkMatrix );
        Graph( Graph *other, std::vector<VariableNode *> *variableNodes );
        ~Graph();
        Graph( const Graph& other );
        Graph& operator=( const Graph& rhs );

        bool CheckConnection( int checkIndex, int variableIndex ) const;

        void SetVariablesFromReal( double *values );
        void SetVariablesFromReal( double *values, double sigma );
        void SetVariablesFromLL( double *ll );
        void GetValues( double *values );
        bool Decode( int maxIterations );
        void DecodeRound();
        void OutputHard( int *guess );
        int GetCheckLength() const;
        int GetVariableLength() const;
        bool CheckSyndrome();

        VariableNode *GetVariable( int index );

        void Debug();
        void ViewGraph();

    protected:
        std::vector<VariableNode *>   variables;
        std::vector<CheckNode *>      checks;

        unsigned numVariables;
        unsigned numChecks;
        bool AllocatedNodes;
        double sigma;
};
