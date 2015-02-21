#include <stdlib.h>
#include <stdio.h>

#define max_degree 30
#define UNASSIGNED -1
#define PI 3.141592653589
#define TRUE 1
#define FALSE 0
#define max_n 64000

struct check_node {
	double val;
	int size;
	double* variables[max_degree+1];
};

struct variable_node {
	double val;
	int size;
	double* checks[max_degree+1];
};

double count_error_rate ( int* data );
double lambda_hat ( int k, int l, double e );
double count_in_errors (int* good, double* noised );
double count_out_errors (int* good, int* final);
double random_g(double std_dev);
double calc_ll(double val, double sigma);
double q_func(double x, double y, double sigma);
double count_ones (double* data, int n);
double gaussian( double x, double sigma );
double find_val (int bin);

void edge_to_node( int* var_list, double* lambda, int len, int max_size );
void permute_array(int *array, int len);
void import_graph( FILE* mat_file, int** matrix, int n, int m);
void find_graph_size( FILE* mat_file, int* n, int* m);
void build_edge_list( int** matrix, int* edge_list, int n, int m );
void init_nodes( struct variable_node* vars, struct check_node* checks, int n, int m_new, int* check_list, int* var_list);
void get_header( FILE* mat_file, int* n, int* m);
void generate_edge_list(int* edge_list, int no_edges, int* var_list);
void generate_matrix(struct variable_node* vars, struct check_node* checks, int n, int m_new, int* edge_list, int** matrix);
void build_graph(struct variable_node* vars, struct check_node* checks, int n, int m_new, int* edge_list);
void write_graph( FILE* mat_file, int** matrix, int n, int m_new );
void build_var_list( int** matrix, int* var_list, int n, int m );
void build_check_list( int** matrix, int* check_list, int n, int m );
void create_puncture_list( int* list, double rate, int n );
void peeling_decode( struct variable_node* vars, struct check_node* checks, int* guess, int n, int m );
void throw_usage();

int make_lists ( int* var_list, int len, double e, int k, int max_size );
int random (int max);
int fix_doubles_list( int* edges, int length, int m_new, int n, int k );
int fix_double_irreg( int* edges, int* check_list, int no_edges, int m, int n, int max );
int update_check_node( struct check_node* check, double sigma );
int update_variable_node( struct variable_node* variable, double sigma );
int hard(double val);
int find_bin (double value);
int make_lists_RSU ( int* var_list, int* check_list, int n, int max );
int make_lists_new ( int* var_list, int* check_list, int n, int max );
int count_edges( int* var_list, int max );
int update_check_nodes_peel( struct check_node* check );
int update_variable_nodes_peel( struct variable_node* variable );
