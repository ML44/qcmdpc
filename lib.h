#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))


/* Index type definition */
typedef uint16_t index_t; // limits the code length to 65536
//typedef uint32_t index_t;


/* Random */
long random(void);
void srandom(unsigned int seed);
int myrnd();
void mysrnd(int seed);


/* Lists */
struct node {
  index_t val;
  struct node * next;
};
typedef struct node * node_t;
struct list {
  int    length;
  node_t index;
};
typedef struct list * list_t;

#define list_length(l) ((l)->length)
list_t list_init( void );
char list_isempty(list_t l);
void list_add(list_t l, index_t v);
void list_remove(list_t l);
void list_print(list_t l, char * str);


/* Lists of lists */
struct list_node {
  list_t val;
  struct list_node * next;
};
typedef struct list_node * list_node_t;
struct list_list {
  int    length;
  list_node_t index;
};
typedef struct list_list * list_list_t;

#define list_list_length(l) ((l)->length)
list_list_t list_list_init( void );
char list_list_isempty(list_list_t l);
void list_list_add(list_list_t l, list_t v);
void list_list_remove(list_list_t l);
void list_list_print(list_list_t l, char * str);


/* Spectral distance */
int spectral_dist(int i, int j, int p);


/* Sparse vectors */
struct sparse_vect {
	int length, weight;
	index_t * coeff;
};
typedef struct sparse_vect * sparse_vect_t;

sparse_vect_t sparse_vect_new(int length, int weight);
void sparse_vect_free(sparse_vect_t h);
void sparse_vect_copy_noalloc(sparse_vect_t copy, sparse_vect_t h);
sparse_vect_t sparse_vect_copy(sparse_vect_t h);
sparse_vect_t sparse_vect_from_indexarray(index_t * v, int length, int weight);
sparse_vect_t sparse_vect_rand_noalloc(sparse_vect_t h, int (*rand_fct)());
sparse_vect_t sparse_vect_rand(int length, int weight, int (*rand_fct)());
sparse_vect_t sparse_vect_xor_noalloc(sparse_vect_t h, sparse_vect_t h0, sparse_vect_t h1);
sparse_vect_t sparse_vect_xor(sparse_vect_t h0, sparse_vect_t h1);


/* Dense vectors */
struct dense_vect {
	int length, weight;
	char * coeff;
};
typedef struct dense_vect * dense_vect_t;

dense_vect_t dense_vect_new(int length);
void dense_vect_free(dense_vect_t synd);
void dense_vect_copy_noalloc(dense_vect_t copy, dense_vect_t s);
dense_vect_t dense_vect_copy(dense_vect_t s);


/* Some macros on vectors */
#define vect_weight(v) ((v)->weight)
#define vect_length(v) ((v)->length)
#define vect_coeff(v, i) ((v)->coeff[i])

