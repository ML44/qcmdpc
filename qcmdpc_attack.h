#include "qcmdpc.h"

// Random
long random(void);
void srandom(unsigned int seed);
int myrnd();
void mysrnd(int seed);

// Qcsynd
void qcsynd_print(qcsynd_t s, char * str);
void qcsynd_print2(qcsynd_t s1, qcsynd_t s2, int l);
void qcsynd_compare(qcsynd_t s1, qcsynd_t s2, int l);
void qcsynd_set_coeff(qcsynd_t s, int i);
char qcsynd_inclusion(qcsynd_t s1, qcsynd_t s2);

// Qcblock
void qcblock_print(qcblock_t h, char * str);
qcblock_t qcblock_or_noalloc(qcblock_t h, qcblock_t h0, qcblock_t h1);

// Distance Counter
typedef uint32_t dist_count_t;
struct node {
  index_t val;
  struct node * next;
};
dist_count_t * dist_count_new(int length);
void dist_count_print(dist_count_t (* counter), int p, char * str);
void dist_count_free(dist_count_t (* counter));
int dist_count_mean(dist_count_t * counter, int p);

// Qclist
typedef struct node * node_t;
struct qclist {
  int    length;
  int    weight;
  node_t index;
};
typedef struct qclist * qclist_t;
#define qclist_weight(l) ((l)->weight)
#define qclist_length(l) ((l)->length)
qclist_t qclist_init(int len);
char qclist_isempty(qclist_t l);
void qclist_add(qclist_t l, index_t v);
void qclist_remove(qclist_t l);
void qclist_print(qclist_t l, char * str);
qclist_t qclist_from_qcblock(qcblock_t h);
qcblock_t qcblock_from_qclist(qclist_t l);

// Distance Spectrum
qcsynd_t spectrum(qcblock_t e);
void spectrum_add_to_counter(dist_count_t (* counter), qcsynd_t spectre, int ws);
qcsynd_t spectrum_from_counter(dist_count_t * counter, int p, int m);
char spectrum_test_new_bit(qcsynd_t s, qclist_t k, int i);
char block_from_spectrum_aux(qcsynd_t spectre, qclist_t k, int w, int b);
qcblock_t block_from_spectrum(qcsynd_t spectre, int w);

// Test functions
void test_spectrum_reconstruction(int p, int bl, int bw, int t, int N, int seuil, int se, int sH);
void test_block_reconstruction(int length, int weight, int seed);

