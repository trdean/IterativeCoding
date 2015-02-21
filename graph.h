#include <vector>
#include "node.h"

#define MAX_ITER_DEFAULT 30

class Graph
{
    public:
        Graph( int dNumVariables, int dNumChecks, int **checkMatrix );
        //~Graph();
        //Graph( const Graph& other );
        //Graph& operator=( const Graph& rhs );

        void SetVariablesFromReal( double *values, double sigma );
        void SetVariablesFromLL( double *ll );
        bool Decode( int maxIterations );
        void OutputHard( int *guess );

    private:
        vector<VariableNode>   variables;
        vector<CheckNode>      checks;

        int numVariables;
        int numChecks;
}
