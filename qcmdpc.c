#include "qcmdpc.h"



qcblock_t qcblock_new(int length, int weight) {
  return sparse_vect_new(length, weight);
}

void qcblock_free(qcblock_t h) {
  sparse_vect_free(h);
}

void qcblock_copy_noalloc(qcblock_t copy, qcblock_t h) {
  sparse_vect_copy_noalloc(copy, h);
}

qcblock_t qcblock_copy(qcblock_t h) {
  return sparse_vect_copy(h);
}

qcblock_t qcblock_from_indexarray(index_t * v, int length, int weight) {
  return sparse_vect_from_indexarray(v, length, weight);
}

qcblock_t qcblock_rand_noalloc(qcblock_t h, int (*rand_fct)()) {
  return sparse_vect_rand_noalloc(h, (*rand_fct)());
}

qcblock_t qcblock_rand(int length, int weight, int (*rand_fct)()) {
  return sparse_vect_rand( length, weight, (*rand_fct)());
}



/* Ou est xor noalloc ? */

/* qcblock_t qcblock_xor(qcblock_t h0, qcblock_t h1) { */
/* 	return qcblock_xor_noalloc(qcblock_new(h0->length, 0), h0, h1); */
/* } */







qcsynd_t qcsynd_new(int length) {
  return dense_vect_new(length);
}

void qcsynd_free(qcsynd_t synd) {
  dense_vect_free(synd);
}

void qcsynd_copy_noalloc(qcsynd_t copy, qcsynd_t s) {
  dense_vect_copy_noalloc(copy, s);
}

qcsynd_t qcsynd_copy(qcsynd_t s) {
  return dense_vect_copy(s);
}
















/* qcmdpc_t is a set of INDEX blocks (INDEX is a macro) representing a
	 QC-MDPC code. Each of the INDEX qcblock_t is the sparse
	 representation of the first column of the corresponding circulant
	 block.

	 Warning: qcmdpc_new(length, weight) allocates INDEX blocks of given
	 weight but do not initialize them */
qcmdpc_t qcmdpc_new(int length, int weight) {
	int i;
	qcmdpc_t h = malloc(INDEX * sizeof (struct qcblock));

	for (i = 0; i < INDEX; ++i) {
		h[i].length = length;
		h[i].weight = weight;
		h[i].coeff = (index_t *) malloc(weight * sizeof (index_t));
	}

	return h;
}

void qcmdpc_free(qcmdpc_t h) {
	int i;

	for (i = 0; i < INDEX; ++i) {
		free(h[i].coeff);
	}
	free(h);
}



/* allocate and pick a random QC-MDPC code formed of INDEX circulant
 blocks of given length and weight. The i-th circulant block of H is
 represented by its first column. */
qcmdpc_t qcmdpc_rand(int length, int weight, int (*rand_fct)()) {
	int i;
	qcmdpc_t H = qcmdpc_new(length, weight);

	for (i = 0; i < INDEX; ++i) {
		qcblock_rand_noalloc(qcmdpc_block(H, i), rand_fct);
	}

	return H;
}













/* add to the syndrome synd the j-th cyclic shift of the column vector h */
qcsynd_t qcmdpc_synd_add(qcsynd_t synd, qcblock_t h, int j) {
	int i, l;
	char c;

	for (l = 0; l < vect_weight(h); ++l) {
		i = j + vect_coeff(h, l);
		if (i >= vect_length(h))
			i -= vect_length(h);
		c = qcsynd_flip_coeff(synd, i); // TODO
		vect_weight(synd) += 2 * c - 1;
	}

	return synd;
}

/* add to the syndrome synd the syndrome of the error e in the QC-MDPC
	 code H */
qcsynd_t qcmdpc_synd_adjust(qcsynd_t synd, qcmdpc_t H, qcblock_t e) {
	int i, j, l, p, offset;
	qcblock_t h;

	h = qcmdpc_block(H, 0);
	p = vect_length(h);
	for (offset = 0, i = 0, l = 0; i < vect_weight(e); ++i) {
		j = vect_coeff(e, i) - offset;
		if (j >= p) {
			j -= p;
			offset += p;
			++l;
			h = qcmdpc_block(H, l);
		}
		qcmdpc_synd_add(synd, h, j);
	}

	return synd;
}

/* allocate and return the syndrome of the error e in the QC-MDPC code
	 H */
qcsynd_t qcmdpc_synd(qcmdpc_t H, qcblock_t e) {
	return qcmdpc_synd_adjust(qcsynd_new(qcmdpc_block_length(H)), H, e);
}








/* computes a single counter value for a position j, with
	 0 <= j < codelength */
/* counter_t qcmdpc_counter(qcmdpc_t H, qcsynd_t synd, int j) { */
/* 	int i, l, p; */
/* 	counter_t w; */
/* 	qcblock_t h; */

/* 	w = 0; */
/* 	p = qcmdpc_block_length(H); */
/* 	h = qcmdpc_block(H, j / p); */
/* 	j %= p; */
/* 	for (l = 0; l < p; ++l) { */
/* 		i = j + qcblock_index(h, l); */
/* 		if (i >= p) */
/* 			i -= p; */
/* 		w += qcsynd_coeff(synd, i); */
/* 	} */

/* 	return w; */
/* } */

/* computes all counter values, returns an array of counters of size
	 codelength */
/* counter_t * qcmdpc_count(qcmdpc_t H, qcsynd_t synd) { */
/* 	int i, j, l, u, p; */
/* 	counter_t * counter, * temp; */
/* 	qcblock_t h; */

/* 	p = qcmdpc_block_length(H); */
/* 	counter = (counter_t *) calloc(INDEX * p, sizeof (counter_t)); */

/* 	for (i = 0; i < p; ++i) { */
/* 		if (qcsynd_coeff(synd, i)) { */
/* 			for (temp = counter, u = 0; u < INDEX; ++u, temp += p) { */
/* 				h = qcmdpc_block(H, u); */
/* 				for (l = 0; l < qcblock_weight(h); ++l) { */
/* 					j = i - qcblock_index(h, l); */
/* 					if (j < 0) */
/* 						j += p; */
/* 					temp[j]++; */
/* 				} */
/* 			} */
/* 		} */
/* 	} */

/* 	return counter; */
/* } */
