/*
 * awgn.c
 *
 * Some functions to help with LDPC codes over a AWGN channel
 *
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

//Polar method of generating gaussian variables given by Knuth
double random_g(double std_dev) {
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

//Return log-likelihood ratio
double calc_ll(double val, double sigma) {
	if( val != 0 )
		return log( q_func(val, 1, sigma) / q_func(val, -1, sigma) );
	else
		return 0;
}

//In case you want to do hard decisions instead of BP
int hard(double val) {
	if(val >= 0)
		return 1;
	else
		return -1;
}

//I don't use this right now...
int indicator( int* v, int size ) {
	int val = 1;
	for ( int i = 0; i < size; i++ )
		val *= v[i];
	if ( val == 1 )
		return 1;
	else
		return 0;
}

double q_func(double x, double y, double sigma) {
	double norm = 1 / sqrt(2 * PI ) * sigma;
	double expon = ( -1 * (y-x) * (y-x) / (2*sigma*sigma) );
	return (double) norm * exp(expon);
}

double gaussian( double x, double sigma ) {
	double norm = 1 / sqrt(2 * PI ) * sigma;
	double expon = ( -1 * x * x / (2*sigma*sigma) );
	return (double) norm * exp(expon);
}
