/*
 * import.c
 *
 *  Created on: May 19, 2012
 *      Author: tom
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"


void import_graph( FILE* mat_file, int** matrix, int n, int m) {
	for( int i = 0; i < m; i++ ) {
		int line_size = 2*n + 2;
		char* buffer = malloc((line_size)*sizeof(char));
		fgets(buffer, line_size, mat_file);

		for( int j = 0; j < n; j++ ) {
			char t = buffer[2*j];
			if( buffer[2*j] == '0' )
				matrix[i][j] = 0;
			else if( buffer[2*j] == '1' )
				matrix[i][j] = 1;
			else {
				matrix[i][j] = 0;
				fprintf(stderr, "Bad value encountered parsing matrix\n");
			}
		}
	}
}


void find_graph_size( FILE* mat_file, int* n, int* m) {

	int max_line_size = 2*max_n + 2;
	char* buffer = malloc( max_line_size*sizeof(char) );
	fgets(buffer, max_line_size, mat_file);

	int line_size = 0;
	while( *(++buffer) != '\n') {
		if( line_size == max_n ) {
			fprintf(stderr, "n larger than %d!\n", max_n );
			buffer -= line_size;
			break;
		}
		line_size++;
	}
	buffer -= line_size;
	line_size += 3; //We want fgets to get the new_line

	*n = (line_size - 1) / 2;

	*m = 1;
	while( fgets(buffer, line_size, mat_file) != NULL )
		(*m)++;

	rewind( mat_file );
}

void build_edge_list( int** matrix, int* edge_list, int n, int m ) {
	int count = 0;
	for( int i = 0; i < m; i++ ) {
		for( int j = 0; j < n; j++ ) {
			if( matrix[i][j] == 1 )
				edge_list[count++] = j;
		}
	}
}

void get_header( FILE* mat_file, int* n, int* m ) {
	char buffer[255];
	fgets(buffer, sizeof buffer, mat_file);

	*n = atoi(strtok( buffer, "," ) );
	*m = atoi(strtok( NULL, "," ) );
}

void write_graph( FILE* mat_file, int** matrix, int n, int m_new ) {
	for( int i = 0; i < m_new; i++ ) {
		for( int j = 0; j < n; j++ ) {
			fprintf(mat_file, "%d,", matrix[i][j]);
		}
		fprintf(mat_file, "\n");
	}
}

void generate_matrix(struct variable_node* vars, struct check_node* checks, int n, int m_new, int* edge_list, int** matrix) {
	int count = 0;
	for(int i = 0; i < m_new; i++ ) {
		for( int j = 0; j < checks[i].size; j++ ) {
			matrix[i][edge_list[count]] = 1;
			count++;
		}
	}
}

void build_var_list( int** matrix, int* var_list, int n, int m ) {
	for( int j = 0; j < n; j++ ) {
		int count = 0;
		for( int i = 0; i < m; i++ ) {
			if( matrix[i][j] == 1 )
				count++;
		}
		if( count < max_degree ) {
			var_list[count - 2]++;
		} else {
			fprintf(stderr, "Variable node %d has too high degree\n", j);
			var_list[max_degree - 1]++;
		}
	}
}

void build_check_list( int** matrix, int* check_list, int n, int m ) {
	for( int i = 0; i < m; i++ ) {
		int count = 0;
		for( int j = 0; j < n; j++ ) {
			if( matrix[i][j] == 1 )
				count++;
		}
		if( count < max_degree ) {
			check_list[count - 3]++;
		} else {
			fprintf(stderr, "Check node %d has too high degree\n", i);
			check_list[max_degree - 1]++;
		}
	}
}
