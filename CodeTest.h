#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

class CodeTest {

    public:
        CodeTest();
        //~CodeTest();
        //CodeTest( const CodeTest& other );
        //CodeTest& operator=( const CodeTest& rhs);

        void TestCode( Graph *TestGraph,
                       double sigma_start, 
                       double sigma_stop, 
                       double step,
                       int max_iterations,
                       int sample_size, 
                       FILE *output );

    private:
        double RandomGaussian( double std_dev );
        double CalculateBER( Graph *TestGraph ); 
        //extern FILE *stdout;
};
