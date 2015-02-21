/*
 * libgraph.c
 *
 *  Created on: May 19, 2012
 *      Author: tom
 */

#include "graph.h"
#include <stdlib.h>
#include <stdio.h>

//Permute a one-dimensional list
//At each element, swap to random location
void permute_array(int *array, int len)
{
	for(int i = 0; i < len; i++) {
		int j = random(len-i);
		int temp = array[i];
		array[i] = array[i+j];
		array[i+j] = temp;
	}
}


//Draw uniformly over [0,max)
int random (int max) {
	double uni = rand() * ( 1.0 / (RAND_MAX + 1.0) );
	return (int) ( uni * (double) max );
}

//Look at the edge list for double edges
//Compare k edge at a time and see if they go to the same place
//If so swap them to a different variable node
int fix_doubles_list( int* edges, int length, int m_new, int n, int k ){
	int changed = 0;
	for( int i = 0 ; i < n; i++ ) {
		for( int j = 0; j < m_new; j++ ) {
			int count = 0;
			for( int a = 0; a < k; a++ ) {
				if( edges[k*j+a] == i ) count++;
				if( count == 2 ) {
					changed = 1;
					int new = random(m_new);
					while( new == j ) {
						new = random(m_new);
					}
					int temp = edges[k*j+a];
					edges[k*j+a] = edges[k*new+a];
					edges[k*new+a] = temp;
					count--;
				}
			}
		}
	}
	return changed;
}

int fix_double_irreg( int* edges, int* check_list, int no_edges, int m, int n, int max ) {
	int changed = 0;

	for( int i = 0; i < n; i++ ) {
		int edge_count = 0;
		for( int degree = 0; degree < max; degree++ ) {
			for( int j = 0; j < check_list[degree]; j++ ) {
				int val_count = 0;
				for( int a = 0; a < degree + 3; a++ ) {
					if( edge_count == no_edges ) break;
					if( edges[edge_count] == i ) val_count++;
					if( val_count == 2 ) {
						changed++;
						int new = random(no_edges);
						int temp = edges[edge_count];
						edges[edge_count] = edges[new];
						edges[new] = temp;
						val_count--;
					}
					edge_count++;
				}
			}
		}
	}
	//fprintf(stderr, "Found %d doubles\n", changed);
	return changed;
}


//Check the parity at the check node and report if it was updated
int update_check_node( struct check_node* check, double sigma ) {

	double init = check->val;
	double product = 1;

	for ( int i = 0; i < check->size; i++ ) {
		double val = *(check->variables[i]);
		if ( val != 0 )
			product *= tanh(  val / 2   );
	}

	check->val = 2*atanh(product);

	if( check->val > 100000 )
		check->val = 100000;
	if( check->val < -100000 )
		check->val = -100000;

	if( init == check->val )
		return 0;
	else
		return 1;
}

//Update the variable node if possible and report if it was updated
int update_variable_node( struct variable_node* variable, double sigma ) {
	double init = variable->val;

	double sum =  init;
	int sign = 1;
	for ( int i = 0; i < variable->size; i++ ) {
		double val =  *(variable->checks[i]);
		sum += fabs(val);
	}

	variable->val = (double) sum;
	if( variable->val > 100000 )
		variable->val = 100000;
	if( variable->val < -100000 )
		variable->val = -100000;
	if( variable->val == init )
		return 0;
	else
		return 1;
}
/*
//Check the parity at the check node and report if it was updated
int update_check_node( struct check_node* check, double sigma ) {

	double init = check->val;
	double sum = 0;
	int sign = 1;

	for ( int i = 0; i < check->size; i++ ) {
		double val = *(check->variables[i]);
		if ( val < 0 )
			sign *= -1;
		sum += log( tanh( fabs( val ) / 2   ) );
	}

	check->val = (double) sign * 2*atanh( exp( sum ) );

	if( check->val > 100000 )
		check->val = 100000;
	if( check->val < -100000 )
		check->val = -100000;

	if( init == check->val )
		return 0;
	else
		return 1;
}

//Update the variable node if possible and report if it was updated
int update_variable_node( struct variable_node* variable, double sigma ) {
	double init = variable->val;

	double sum = init;
	int sign = 1;
	for ( int i = 0; i < variable->size; i++ ) {
		double val =  *(variable->checks[i]);
		sum += val;
	}

	variable->val = (double) sum;
	if( variable->val > 100000 )
		variable->val = 100000;
	if( variable->val < -100000 )
		variable->val = -100000;
	if( variable->val == init )
		return 0;
	else
		return 1;
}
*/

//Return the percentage of ones in the data
double count_ones (double* data, int n) {
	int count = 0;
	for ( int i = 0; i < n; i++ ) {
		if( data[i] == 1 )
			count++;
	}
	return (double) count / n;
}

void init_nodes( struct variable_node* vars, struct check_node* checks, int n, int m_new, int* check_list, int* var_list) {

	int count = 0;

	//Set size of our nodes
	for(int i = 0; i < max_degree; i++) {
		for(int j = 0; j < check_list[i]; j++) {
			checks[count].size = i+3;
			count++;
		}
	}

	count = 0;
	for(int i = 0; i < max_degree; i++) {
		for(int j = 0; j < var_list[i]; j++) {
			vars[count].size = i+2;
			count++;
		}
	}

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < vars[i].size; j++)
			vars[i].checks[j] = UNASSIGNED;
	}
}

//Index edges by the variable node they point to, this will
//be permuted and then they will be assigned
//i.e. if we have a degree of three on our check nodes we would have
//{1, 1, 1, 2, 2, 2, 3, 3, 3, ..., n, n, n}
void generate_edge_list(int* edge_list, int no_edges, int* var_list) {
	int edge_count = 0;
	int node_count = 0;
	for(int i = 0; i < max_degree; i++) {
		for(int j = 0; j < var_list[i]; j++) {
			for(int t = 0; t < i+2; t++) {
				edge_list[edge_count] = node_count;
				edge_count++;
			}
			node_count++;
		}
	}
}

void build_graph(struct variable_node* vars, struct check_node* checks, int n, int m_new, int* edge_list) {
	int count = 0;
	for(int i = 0; i < m_new; i++ ) {
		checks[i].val = 0;
		for( int j = 0; j < checks[i].size; j++ ) {
			checks[i].variables[j] = &vars[edge_list[count]].val;
			int a = 0;
			//Find the first edge of the variable node that hasn't been assigned yet
			while( vars[edge_list[count]].checks[a] != UNASSIGNED ) {
				a++;
				//This should never happen...but i had a bug at one point:
				if( a > vars[edge_list[count]].size ) {
					fprintf(stderr, "Exceeded Dimensions, node: %d, ref: %d\n", edge_list[count], a);
					a--;
					break;
				}
			}
			//Make variables point back to checks and update the matrix
			vars[edge_list[count]].checks[a] = &checks[i].val;
			count++;
		}
	}
}

int count_edges( int* var_list, int max ) {
	int count = 0;
	for( int i = 0; i < max; i++ ) {
		count += (i+2) * var_list[i];
	}

	return count;
}

void create_puncture_list( int* list, double rate, int size ) {
	for( int i = 0; i < size; i++ ) {
		double uni = rand() * ( 1.0 / (RAND_MAX + 1.0) );
		if( uni < rate )
			list[i] = 1;
		else
			list[i] = 0;
	}
}

void peeling_decode( struct variable_node* vars, struct check_node* checks, int* guess, int n, int m ) {
	for(int i = 0; i < m; i++ )
		checks[i].val = -1;
	for( int i = 0; i < n; i++ )
		vars[i].val = (double) guess[i];

	int flag = 1;
	int iter = 0;
	while( flag == 1 && iter < 50 ) {
		iter++;
		int count = 0;
		for( int i = 0; i < m; i++ )
			update_check_nodes_peel(&checks[i]);

		for( int i = 0; i < n; i++ ) {
			if( update_variable_nodes_peel(&vars[i]) == 2 )
				count++;
		}
		if (count == 0)
			flag = 0;
	}

	for( int i = 0; i < n; i++ )
		guess[i] = vars[i].val;
}

int update_check_nodes_peel( struct check_node* check ) {
	int stars = 0;
	int last_star = 0;
	int sum = 0;
	for(int i = 0; i < check->size; i++) {
		int val = (int) *(check->variables[i]);
		if( val == -1 ) {
			stars++;
			last_star = i;
		} else
			sum = sum ^ val;
	}
	if (stars == 0) {
		check->val = 0;
		return 0;
	} else if(stars == 1) {
		check->val = sum;
		return 1;
	} else
		return 2;
}

int update_variable_nodes_peel( struct variable_node* variable ) {
	if( variable->val == -1 ) {
		int flag = 0;
		for( int i = 0; i < variable->size; i++ ) {
			if( *(variable->checks[i]) != -1 ) {
				variable->val =  *(variable->checks[i]);
				flag = 1;
			}
		}
		if( flag == 0 )
			return 2;
		else
			return 1;
	} else
		return 0;
}
