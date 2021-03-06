#ifndef QCV_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//#define VERBOSE

#ifndef INDEX
#define INDEX 2
#endif

typedef uint16_t index_t; // limits the code length to 65536
//typedef uint32_t index_t;

// counter not likely to be > 255
typedef uint8_t counter_t;

// sparse representation for errors and qcmdpc codes
struct qcblock {
	int length, weight;
	index_t * index;
};

typedef struct qcblock * qcblock_t;
typedef qcblock_t qcmdpc_t;

// dense representation for syndromes
struct qcsynd {
	int length, weight;
	char * coeff;
};

typedef struct qcsynd * qcsynd_t;


// dense representation for spectrum
struct spectre {
	int length;
	char * coeff;
};

typedef struct spectre * spectre_t;


// table to count iterations
struct compteur {
	int length;
	int * coeff;
};

typedef struct compteur * compteur_t;



#define qcblock_weight(h) ((h)->weight)
#define qcblock_length(h) ((h)->length)
#define qcblock_index(h, i) ((h)->index[i])

#define qcmdpc_block(H, i) ((H) + (i))
#define qcmdpc_block_length(H) ((H)->length)
#define qcmdpc_block_weight(H) ((H)->weight)

#define qcsynd_coeff(synd, i) ((synd)->coeff[i])
#define qcsynd_flip_coeff(synd, i) ((synd)->coeff[i] ^= 1)
#define qcsynd_weight(synd) ((synd)->weight)
#define qcsynd_length(synd) ((synd)->length)

#define spectre_coeff(s, i) ((s)->coeff[i])
#define spectre_set_coeff(s, i, b) ((s)->coeff[i] = b)
#define spectre_length(s) ((s)->length)

#define compteur_coeff(c, i) ((c)->coeff[i])
#define compteur_add_coeff(c, i, n) ((c)->coeff[i] += n)
#define compteur_length(s) ((c)->length)

qcblock_t qcblock_new(int length, int weight);
void qcblock_free(qcblock_t h);
void qcblock_copy(qcblock_t copy, qcblock_t h);
qcblock_t qcblock_copy_alloc(qcblock_t h);
qcblock_t qcblock_from_indexarray(index_t * v, int length, int weight);
qcblock_t qcblock_rand_noalloc(qcblock_t h, int (*rand_fct)());
qcblock_t qcblock_rand(int length, int weight, int (*rand_fct)());

qcblock_t qcblock_xor_noalloc(qcblock_t h, qcblock_t h0, qcblock_t h1);
qcblock_t qcblock_xor(qcblock_t h0, qcblock_t h1);

qcsynd_t qcsynd_new(int length);
void qcsynd_free(qcsynd_t synd);
void qcsynd_copy(qcsynd_t copy, qcsynd_t s);
qcsynd_t qcsynd_copy_alloc(qcsynd_t s);

spectre_t spectre_new(int length);
void make_spectre_of(spectre_t s, qcblock_t e);
void spectre_free(spectre_t s);

compteur_t compteur_new(int length);
void compteur_add_spectrum(compteur_t c, spectre_t s, int n);
void compteur_free(compteur_t c);

qcmdpc_t qcmdpc_new();
void qcmdpc_free(qcmdpc_t h);
qcmdpc_t qcmdpc_rand(int length, int weight, int (*rand_fct)());
qcsynd_t qcmdpc_synd_add(qcsynd_t synd, qcblock_t h, int j);
qcsynd_t qcmdpc_synd_adjust(qcsynd_t synd, qcmdpc_t H, qcblock_t e);
qcsynd_t qcmdpc_synd(qcmdpc_t H, qcblock_t e);
counter_t qcmdpc_counter(qcmdpc_t H, qcsynd_t synd, int j);
counter_t * qcmdpc_count(qcmdpc_t H, qcsynd_t synd);

#define QCV_H

#endif
