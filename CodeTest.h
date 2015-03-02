#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "product.h"

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

        void TestProduct( Product *TestProduct,
                          double sigma_start,
                          double sigma_stop,
                          double step,
                          int max_iterations,
                          int sample_size,
                          FILE *output );

        void TestDebug( Graph *TestGraph,
                        double sigma,
                        int iterations );

    private:
        double RandomGaussian( double std_dev );
        double CalculateBER( Graph *TestGraph );
        double CalculateBER( Product *TestProduct ); 
        double CalculateUncoded( double *values, int len );
        //extern FILE *stdout;
};
