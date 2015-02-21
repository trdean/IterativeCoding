#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "graph.h"

int main()
{
	int k, l, n, irregular;
	int max_iter, per_graph, import, puncture;
	double p;

	time_t rawtime;
	struct tm * timeinfo;
	char filename [80];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime (filename,80,"%c.csv",timeinfo);

	FILE *output_file;
	output_file = fopen(filename, "w");

	max_iter = 300;
	per_graph = 1000;
	import = FALSE;
	irregular = TRUE;
	puncture = FALSE;


	if( import == FALSE ) {
		k = 20;
		l = 4;
		n = 64000;
	}

	p = (double) 400 / n;

	FILE *matf;

	int* var_list   = (int*) malloc((max_degree)*sizeof(int));
	int* check_list = (int*) malloc((max_degree)*sizeof(int));
	int** matrix;
	int m;
	int* edge_list;
	int no_edges;

	//double* error_by_bit;

	for( int i = 0; i < max_degree; i++ )
		var_list[i] = 0;

	for (int i = 0; i < max_degree - 2; i++)
		check_list[i] = 0;

	//If we are not importing a matrix, generate a random graph.
	if( import == FALSE ) {

		matf = fopen("matf.csv", "w");

		if( irregular == TRUE ) {
			//m = make_lists( var_list, n, 0.97, k, max_degree );
			//check_list[k-3] = m;
			//m = make_lists_RSU ( var_list, check_list, n, max_degree );
			m = make_lists_new ( var_list, check_list, n, max_degree );
		} else {
			var_list[l-2] = n;
			m = n / (k / l);
			check_list[k-3] = m;
		}

		int dim1 = m;
		int dim2 = n;
		matrix = malloc(dim1*sizeof(int*));
		for( int i = 0; i < dim1; i++ ) {
			matrix[i] = malloc(dim2*sizeof(int));
		}

		//error_by_bit = malloc(n * sizeof(double));
		//for ( int i = 0; i < n; i++ )
		//	error_by_bit[i] = 0;

		//fprintf(matf, "%d, %d\n", n, m);

		no_edges = count_edges( var_list, max_degree );

		edge_list = (int *) malloc(no_edges*sizeof(int));
		generate_edge_list(edge_list, no_edges, var_list);

		permute_array(edge_list, no_edges);

		if( irregular == FALSE )
			while( fix_doubles_list( edge_list, no_edges, m, n, k ) != 0 );
		else
			while( fix_double_irreg( edge_list, check_list, no_edges, m, n, max_degree ) != 0 );

	//Otherwise import the graph
	} else {
		matf = fopen("matf.csv", "r");

		find_graph_size( matf, &n, &m );

		//p = (double) 500 / n;
		//var_list[l-2] = n;
		//m = n / (k / l);
		//check_list[k-3] = m;

		int dim1 = m;
		int dim2 = n;
		matrix = malloc(dim1*sizeof(int*));
		for( int i = 0; i < dim1; i++ ) {
			matrix[i] = malloc(dim2*sizeof(int));
		}

		//error_by_bit = malloc(n * sizeof(double));
		//for ( int i = 0; i < n; i++ )
		//	error_by_bit[i] = 0;

		no_edges = k*m;
		edge_list = (int *) malloc(no_edges*sizeof(int));

		import_graph( matf, matrix, n, m);

		build_var_list( matrix, var_list, n, m );
		build_check_list( matrix, check_list, n, m );

		build_edge_list( matrix, edge_list, n, m );
	}


	FILE *bit_file;
	bit_file = fopen("biterror.csv", "w" );

	if( puncture == TRUE ) {
		printf("Rate: %3.4f\n", p + 1 - (double) m / n);
		printf("Rate*: %3.4f\n", 1 - p - (double) m / n);
	}
	else
		printf("Rate: %3.4f\n", 1 - (double) m / n);

	//Seed PRNG
	unsigned int seed = (unsigned int)time(NULL);
	srand(seed);

	//Used to count errors
	double e_in = 0.0;
	double e_out = 0.0;

	struct variable_node* vars = malloc(n*sizeof(struct variable_node));
	struct check_node* checks  = malloc(m*sizeof(struct check_node));

	init_nodes(vars, checks, n, m, check_list, var_list);

	if( import == FALSE ) {
		generate_matrix(vars, checks, n, m, edge_list, matrix);
		write_graph( matf, matrix, n, m );
	}

	build_graph(vars, checks, n, m, edge_list);

	//Main work is here
	printf("Decoding...\n");
	//double sigma = 0.62;
	char time_now [80];
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime (time_now,80,"%X",timeinfo);
	printf("Start time: %s\n", time_now);
	for ( double sigma = 0.50; sigma < 0.81; sigma += 0.01 ) {
	//for ( max_iter = 30; max_iter < 300; max_iter += 5 ) {

		e_out = 0;

		int* puncture_list = malloc( n*sizeof(int) );
		for ( int i = 0; i < n; i++ )
			puncture_list[i] = 0;
		if( puncture == TRUE ) {
			create_puncture_list( puncture_list, p, n );
		}

		for( int run = 0; run < per_graph; run++ ) {

			//if( run % 1000 == 0 )
			//	fprintf(stderr, "%d%\n", run / 1000 );

			//Send an message of all zeros
			int info[n];
			for(int i = 0; i < n; i++) {
				info[i] = 1;
			}

			//Add noise
			double errored[n];
			for(int i = 0; i < n; i++) {
				double noise = random_g(sigma);
				errored[i] = ((double) info[i]) + noise;
			}

			//Reset check nodes
			for(int i = 0; i < m; i++ ) {
				checks[i].val = 0;
			}

			//Put data in variables in form of log-likelihoods
			for(int i = 0; i < n; i++) {
				if( puncture_list[i] == 1 )
					vars[i].val = 0;
				else
					vars[i].val = calc_ll(errored[i], sigma);
			}

			//Check our starting error rate
			//e_in = count_ones(&errored);

			//Now decode until we hit our max iterations or nothing gets changed
			int flag = 1;
			int iter = 0;
			while( iter < max_iter && flag == 1 ) {
				iter++;
				int count = 0;
				for( int i = 0; i < m; i++ )
					count += update_check_node(&checks[i], sigma);

				for( int i = 0; i < n; i++ ){
					if( puncture_list[i] == 0 )
						count += update_variable_node(&vars[i], sigma);
				}

				if (count == 0)
					flag = 0;
			}

			//Update our data
			double guess[n];
			for ( int i = 0; i < n; i++ ) {
				if( vars[i].val >= 0 )
					guess[i] = 0;
				//else if( puncture_list[i] == 1 )
				//	guess[i] = -1;
				else{
					guess[i] = 1;
					e_out++;
					//error_by_bit[i] += 1;
				}
			}

			//if( puncture == TRUE )
			//	peeling_decode( vars, checks, guess, n, m );

			//for( int i = 0; i < n; i++ ) {
			//	if( guess[i] == 1 )
			//		e_out++;
			//}
		}

		e_out = e_out / ( (double) per_graph * (n) );

		//for( int i = 0; i < n; i++ ) {
		//	error_by_bit[i] = (double) error_by_bit[i] / per_graph;
		//	fprintf(bit_file, "%3.4f,", error_by_bit[i]);
		//} fprintf(bit_file, "\n");

		//Output everything
		printf("Sigma: %3.2f, Avg. Err: %g\n", sigma, e_out );
		fprintf(output_file, "%3.2f, %g\n", sigma, e_out);
		char time_now [80];
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		strftime (time_now,80,"%X",timeinfo);
		printf("Stop time: %s\n", time_now);
		free(puncture_list);
	}


	//Garbage collection
	free(vars);
	vars = NULL;

	free(checks);
	checks = NULL;

	//free(edge_list);
	//edge_list = NULL;

	//free(error_by_bit);
	//error_by_bit = NULL;

	for( int i = 0; i < m; i++ )
		free(matrix[i]);
	free(matrix);
	matrix = NULL;

	fclose(output_file);
	fclose(matf);
	fclose(bit_file);
	return 0;
}

void throw_usage() {
	printf("Simulates LDPC codes over AWGN Channel.  Usage:\n");
	printf("-i filename         Imports code from filename\n");
	printf("-r l k              Use regular (l,k) ensemble\n");
	printf("-irregular          Use irregular ensemble\n");
	printf("-n                  Specifies block length\n");
	printf("-h                  Prints this help\n");
}
