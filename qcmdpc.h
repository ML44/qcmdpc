#include "lib.h"

#ifndef INDEX
#define INDEX 2
#endif



// sparse representation for errors and qcmdpc codes
typedef sparse_vect_t qcblock_t;
typedef qcblock_t qcmdpc_t;

// dense representation for syndromes
typedef struct dense_vect_t qcsynd_t;

#define qcmdpc_block(H, i) ((H) + (i))

qcblock_t qcblock_new(int length, int weight);
void qcblock_free(sparse_vect_t h);
void qcblock_copy_noalloc(qcblock_t copy, qcblock_t h);
qcblock_t qcblock_copy(qcblock_t h);
qcblock_t qcblock_from_indexarray(index_t * v, int length, int weight);
qcblock_t qcblock_rand_noalloc(qcblock_t h, int (*rand_fct)());
qcblock_t qcblock_rand(int length, int weight, int (*rand_fct)());

/* qcblock_t qcblock_xor_noalloc(qcblock_t h, qcblock_t h0, qcblock_t h1); */
/* qcblock_t qcblock_xor(qcblock_t h0, qcblock_t h1); */

qcsynd_t qcsynd_new(int length);
void qcsynd_free(qcsynd_t synd);
void qcsynd_copy(qcsynd_t copy, qcsynd_t s);
qcsynd_t qcsynd_copy_alloc(qcsynd_t s);

qcmdpc_t qcmdpc_new();
void qcmdpc_free(qcmdpc_t h);
qcmdpc_t qcmdpc_rand(int length, int weight, int (*rand_fct)());

qcsynd_t qcmdpc_synd_add(qcsynd_t synd, qcblock_t h, int j);
qcsynd_t qcmdpc_synd_adjust(qcsynd_t synd, qcmdpc_t H, qcblock_t e);
qcsynd_t qcmdpc_synd(qcmdpc_t H, qcblock_t e);



/* // counter not likely to be > 255 */
/* typedef uint8_t counter_t; */
/* counter_t qcmdpc_counter(qcmdpc_t H, qcsynd_t synd, int j); */
/* counter_t * qcmdpc_count(qcmdpc_t H, qcsynd_t synd); */
