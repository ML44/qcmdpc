#include "qcmdpc.h"

#ifndef ATTACK_H
#define ATTACK_H

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

// List
list_t list_from_qcblock(qcblock_t h);
qcblock_t qcblock_from_list(list_t l);

// Distance Spectrum
qcsynd_t spectrum(qcblock_t e);
void spectrum_add_to_counters(dist_count_t (* sweight_counter), 
			      dist_count_t (* dist_freq_counter), 
			      qcsynd_t spectre, int sw);
qcsynd_t spectrum_from_counter(dist_count_float_t * counter, int p, float threshold);
int spectrum_dist( int i, int j, int p);

// Reconstruction functions
void block_reconstruction(int length, int weight, int seed);

#endif
