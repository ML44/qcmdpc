#include "qcmdpc_graph.h"

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
dist_count_t * dist_count_new(int length);
void dist_count_print(dist_count_t (* counter), int p, char * str);
void dist_count_free(dist_count_t (* counter));
int dist_count_mean(dist_count_t * counter, int p);

// Distance Counter Float
typedef float dist_count_float_t;
dist_count_float_t * dist_count_float_new(int length);
void dist_count_float_print(dist_count_float_t (* counter), int p, char * str);
void dist_count_float_free(dist_count_float_t (* counter));

// Qclist
qclist_t qclist_from_qcblock(qcblock_t h);
qcblock_t qcblock_from_qclist(qclist_t l);

// Distance Spectrum
qcsynd_t spectrum(qcblock_t e);
void spectrum_add_to_counters(dist_count_t (* sweight_counter), 
			      dist_count_t (* dist_freq_counter), 
			      qcsynd_t spectre, int sw);
qcsynd_t spectrum_from_counter(dist_count_float_t * counter, int p, float threshold);
char spectrum_test_new_bit(qcsynd_t s, qclist_t k, int i);
char block_from_spectrum_aux(qcsynd_t spectre, qclist_t k, int w, int b);
qcblock_t block_from_spectrum(qcsynd_t spectre, int w);
int spectrum_dist( int i, int j, int p);


// Data functions
void syndrom_weight_distribution(int p, int w, int t, int d, int N, int se, int sh);

// Reconstruction functions
void spectrum_reconstruction(int p, int w, int t, int N, int se, int sh);
void block_reconstruction(int length, int weight, int seed);

qcsynd_t observe_spectrum(qcmdpc_t H, int p, int w, int t, int N, int se, float threshold);

