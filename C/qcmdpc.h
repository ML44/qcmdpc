#include "lib.h"

#ifndef INDEX
#define INDEX 2
#endif

#ifndef QCMDPC_H
#define QCMDPC_H

// sparse representation for errors
typedef sparse_vect_t qcblock_t;
qcblock_t qcblock_new(int length, int weight);
void qcblock_free(sparse_vect_t h);
void qcblock_copy_noalloc(qcblock_t copy, qcblock_t h);
qcblock_t qcblock_copy(qcblock_t h);
qcblock_t qcblock_from_indexarray(index_t * v, int length, int weight);
qcblock_t qcblock_rand_noalloc(qcblock_t h, int (*rand_fct)());
qcblock_t qcblock_rand(int length, int weight, int (*rand_fct)());
qcblock_t qcblock_xor_noalloc(qcblock_t h, qcblock_t h0, qcblock_t h1);
qcblock_t qcblock_xor(qcblock_t h0, qcblock_t h1);

// sparse representation for qcmdpc codes
typedef qcblock_t qcmdpc_t;
#define qcmdpc_block(H, i) ((H) + (i))
#define qcmdpc_block_length(H) ((H)->length)
#define qcmdpc_block_weight(H) ((H)->weight)
qcmdpc_t qcmdpc_new();
void qcmdpc_free(qcmdpc_t h);
qcmdpc_t qcmdpc_rand(int length, int weight, int (*rand_fct)());


// dense representation for syndromes
typedef dense_vect_t qcsynd_t;
#define qcsynd_flip_coeff(synd, i) ((synd)->coeff[i] ^= 1)
qcsynd_t qcsynd_new(int length);
void qcsynd_free(qcsynd_t synd);
void qcsynd_copy_noalloc(qcsynd_t copy, qcsynd_t s);
qcsynd_t qcsynd_copy(qcsynd_t s);
void qcsynd_xor_noalloc(qcsynd_t s1, qcsynd_t s2, qcsynd_t s);
qcsynd_t qcsynd_xor(qcsynd_t s1, qcsynd_t s2);

// syndrom computation
qcsynd_t qcmdpc_synd_add(qcsynd_t synd, qcblock_t h, int j);
qcsynd_t qcmdpc_synd_adjust(qcsynd_t synd, qcmdpc_t H, qcblock_t e);
qcsynd_t qcmdpc_synd(qcmdpc_t H, qcblock_t e);
qcsynd_t qcmdpc_synd_noalloc(qcsynd_t s, qcmdpc_t H, qcblock_t e);

// counter not likely to be > 255
typedef uint8_t counter_t;
counter_t qcmdpc_counter(qcmdpc_t H, qcsynd_t synd, int j);
counter_t * qcmdpc_count(qcmdpc_t H, qcsynd_t synd);

#endif
