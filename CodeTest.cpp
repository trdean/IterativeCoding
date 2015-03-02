#include <time.h>
#include <math.h>
#include "CodeTest.h"

CodeTest::CodeTest() 
{
    unsigned int seed;

    seed = (unsigned int) time(NULL);
    srand(seed); 
}

void CodeTest::TestCode( Graph *TestGraph,
                    double sigma_start,
                    double sigma_stop,
                    double step = 0.1,
                    int max_iterations = 30,
                    int sample_size = 1000,
                    FILE *output = stdout )
{
    double sigma, BER, Uncoded_BER;
    int i,j;

    fprintf( output, "Sigma,    BER,      Uncoded BER\n" );

    double *values = (double *) malloc( TestGraph->GetVariableLength() * 
                                        sizeof(double) );

    for ( sigma = sigma_start; sigma <= sigma_stop; sigma += step ) {
        fprintf( output, "%f, ", sigma );
        BER = 0.0;
        Uncoded_BER = 0.0;
        for ( i = 0; i < sample_size; i++ ) {
            //Generate random samples; simulate sending all zeros
            for ( j = 0; j < TestGraph->GetVariableLength(); j++ ) {
                values[j] = RandomGaussian( sigma ) - 1;
            }

            //Set values on graph
            TestGraph->SetVariablesFromReal( values, sigma );

            TestGraph->Decode( max_iterations );

            BER += CalculateBER( TestGraph );
            Uncoded_BER += CalculateUncoded( values, 
                                             TestGraph->GetVariableLength() );

            //Output
        }
        BER = BER / (double) sample_size; 
        Uncoded_BER = Uncoded_BER / (double) sample_size;
        fprintf( output, "%f, %f\n", BER, Uncoded_BER );
    }

    free( values );
}

void CodeTest::TestProduct( Product *TestProduct,
                    double sigma_start,
                    double sigma_stop,
                    double step = 0.1,
                    int max_iterations = 30,
                    int sample_size = 1000,
                    FILE *output = stdout )
{
    double sigma, BER, Uncoded_BER;
    int i,j;

    fprintf( output, "Sigma,    BER,      Uncoded BER\n" );

    double *values = (double *) malloc( TestProduct->GetVariableLength() * 
                                        sizeof(double) );

    for ( sigma = sigma_start; sigma <= sigma_stop; sigma += step ) {
        fprintf( output, "%f, ", sigma );
        BER = 0.0;
        Uncoded_BER = 0.0;
        for ( i = 0; i < sample_size; i++ ) {
            //Generate random samples; simulate sending all zeros
            for ( j = 0; j < TestProduct->GetVariableLength(); j++ ) {
                values[j] = RandomGaussian( sigma ) - 1;
            }

            //Set values on graph
            TestProduct->SetVariablesFromReal( values, sigma );

            TestProduct->Decode( max_iterations );

            BER += CalculateBER( TestProduct );
            Uncoded_BER += CalculateUncoded( values, 
                                             TestProduct->GetVariableLength() );

            //Output
        }
        BER = BER / (double) sample_size; 
        Uncoded_BER = Uncoded_BER / (double) sample_size;
        fprintf( output, "%f, %f\n", BER, Uncoded_BER );
    }

    free( values );
}

void CodeTest::TestDebug( Graph *TestGraph,
                          double sigma,
                          int iterations )
{
    int i;
    int vars = TestGraph->GetVariableLength();

    double *values = (double *) malloc( vars * sizeof(double) );

    printf("Values\n");

    for ( i = 0; i < vars; i++ ) {
        values[i] = RandomGaussian( sigma ) - 1;
        printf("%f\n", values[i]);
    }

    TestGraph->SetVariablesFromReal( values, sigma );
    TestGraph->Debug();

    for ( i = 0; i < iterations; i++ ) {


        TestGraph->DecodeRound();
        TestGraph->Debug();
    }

    free( values );                                                 
}

//Polar method of generating Gaussian variables, given by Knuth
double CodeTest::RandomGaussian( double std_dev )
{
    static double V1, V2, S;
    static int phase = 0;
    double X;

    if(phase == 0) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);

    phase = 1 - phase;

    return (double) X*std_dev;
}

double CodeTest::CalculateBER( Graph *TestGraph )
{
    int i, number_incorrect, number_variables;

    number_variables = TestGraph->GetVariableLength(); 

    int *guess = (int *) malloc( number_variables * sizeof( int ) );

    TestGraph->OutputHard( guess );

    number_incorrect = 0;

    for ( i = 0; i < number_variables; i++ )
        number_incorrect += guess[i];

    free( guess );

    return (double) number_incorrect / (double) number_variables;
}

double CodeTest::CalculateBER( Product *TestProduct )
{
    int i, number_incorrect, number_variables;

    number_variables = TestProduct->GetVariableLength(); 

    int *guess = (int *) malloc( number_variables * sizeof( int ) );

    TestProduct->OutputHard( guess );

    number_incorrect = 0;

    for ( i = 0; i < number_variables; i++ )
        number_incorrect += guess[i];

    free( guess );

    return (double) number_incorrect / (double) number_variables;
}

double CodeTest::CalculateUncoded( double *values, int len )
{
    int i, number_incorrect;

    number_incorrect = 0;

    for ( i = 0; i < len; i++ ) {
        if ( values[i] > 0 )
            number_incorrect++;
    }

    return (double) number_incorrect / (double) len;
}
