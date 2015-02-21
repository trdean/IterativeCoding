/*
 * Some functions to generate irregular distributions that achieve
 * capacity in a BEC
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graph.h"

//Make a list that has a total of n variable nodes that are optimized to meet
//within 1-e of capacity.  Maximum degree of variable nodes is constrained by max_size
int make_lists( int* var_list, int len, double e, int k, int max_size ) {
	double* lambda = (double *) malloc(max_size*sizeof(double));

	for(int i = 0; i < max_size; i++ ) {
		lambda[i] = 0;
	}

	//Calculate lambda until the list sizes to one or we hit
	//our maximum degree
	double sum = 0;
	int max_deg = 2;
	int count = 0;
	while(sum < 1.0 && max_deg <= max_size + 1) {
		lambda[count] += lambda_hat( k, max_deg, e );
		sum += lambda[count];
		count++;
		max_deg++;
	}

	//Normalize the distribution
	for(int i = 0; i < max_deg - 2; i++) {
		lambda[i] = lambda[i] / sum;
	}

	//Convert to node perspective and store in variable list
	edge_to_node(var_list, lambda, len, max_size);

	free (lambda);
	lambda = NULL;

	//We now have to find the nearest value of m to use and then
	//tweak the number of edges so they are each on each side.
	//I do this by just getting rid of edges on the highest degree
	//variable nodes
	int edge_no = 0;
	int max_real = 0;
	for(int i = 0; i < max_size; i++) {
		edge_no += (i+2) * var_list[i];
		if ( var_list[i] != 0 )
			max_real = i;
	}

	int m_new = edge_no / k;
	int diff = edge_no - m_new * k;

	if( diff != 0 ) {
		var_list[max_real]--;
		var_list[max_real-diff]++;
	}

	return m_new;
}

int make_lists_RSU ( int* var_list, int* check_list, int n, int max ) {
	int m = n / 2;

	double lambda[] = { 0.21991, 0.23328, 0.02058, 0, 0.08543, 0.06540, 0.04767, 0.01912, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0.08064, 0.22798 };
	double rho[] = { 0, 0, 0, 0, 0, 0.64854, 0.34747, 0.00399 };

	edge_to_node( var_list, lambda, n, 19 );
	edge_to_node( check_list, rho, m , 9 );

	int check_edges = 0;
	int var_edges = 0;
	int max_var = 0;
	int max_check = 0;

	for( int i = 0; i < 19; i++ ) {
		var_edges += (i+2) * var_list[i];
		if ( var_list[i] != 0 )
			max_var = i;
	}

	for( int i = 0; i < 8; i++ ) {
		check_edges += (i+3) * check_list[i];
		if ( check_list[i] != 0 )
			max_check = i;
	}

	int diff = var_edges - check_edges;

	if( diff > 0 ) {
		var_list[max_var]--;
		var_list[max_var - diff]++;
	} else if( diff < 0 ) {
		var_list[ -1 * diff ]++;
		var_list[0]--;
	}

	return m;
}

int make_lists_new ( int* var_list, int* check_list, int n, int max ) {

	var_list[1] =  0.95 * n;
	var_list[2] = 0.05 * n;

	int no_edges = var_list[1]*3 + var_list[2]*4;

	int m = no_edges / 20;

	check_list[17] = m;

	int remainder = no_edges % 20;

	return m;
}

//Calculate each lambda_hat according to formula in notes
double lambda_hat( int k, int len, double e ) {
	double kd = (double) k;
	double ld = (double) len;
	double num = pow(-1, ld) * tgamma( (1/(kd-1)) + 1 );
	double den = e * tgamma(ld) * tgamma( (1/(kd-1)) - ld + 2);

	return num / den;
}


//Converts for edge to node perspective and tweaks the list so that we have
//exactly the requested number of variable nodes
void edge_to_node( int* var_list, double* lambda, int len, int max_size ) {
	double average_lambda = 0;
	for( int i = 0; i < max_size; i++ )
		average_lambda += lambda[i]/(i+2);
	int total_n = 0;
	for( int i = 0; i < max_size; i++ ) {
		double norm_vars = lambda[i]/average_lambda;
		var_list[i] = (int) (((len) * norm_vars / (i+2) ) + 0.5);
		total_n += var_list[i];
	}
	if( total_n != len )
		var_list[max_size - 1] += len - total_n;
}
