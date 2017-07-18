#include "qcmdpc.h"

/* qcblock_t is a binary block of given length. The representation
	 favors sparse blocks: it is given as a list of non-zero coordinate
	 indices.

	 Warning: qcblock_new(length, weight) allocates a block of given
	 weight but do not initializes it. If weight=0 then index=NULL. */
qcblock_t qcblock_new(int length, int weight) {
	qcblock_t h = malloc(sizeof (struct qcblock));

	h->length = length;
	h->weight = weight;
	h->index = (index_t *) malloc(weight * sizeof (index_t));

	return h;
}

void qcblock_free(qcblock_t h) {
	free(h->index);
	free(h);
}

void qcblock_copy(qcblock_t copy, qcblock_t h) {
	if (copy->weight != h->weight)
		copy->index = (index_t *) realloc(copy->index, copy->weight * sizeof (index_t));
	memcpy(copy->index, h->index, h->weight * sizeof (index_t));
}

qcblock_t qcblock_copy_alloc(qcblock_t h) {
	qcblock_t copy = qcblock_new(h->length, h->weight);
	memcpy(copy->index, h->index, h->weight * sizeof (index_t));
	return copy;
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
		h[i].index = (index_t *) malloc(weight * sizeof (index_t));
	}

	return h;
}

void qcmdpc_free(qcmdpc_t h) {
	int i;

	for (i = 0; i < INDEX; ++i) {
		free(h[i].index);
	}
	free(h);
}

/* qcsynd_t is the type used to represent a syndrome. In the current
	 implementation, it is assumed that the syndrome is represented as a
	 dense column vector.  If the weight is unknown its value is set to
	 -1.  If the weight is >=0 it is assumed to be correct.

	 qcsynd_new(length) allocates a dense vector of given length and
	 initializes it to null */
qcsynd_t qcsynd_new(int length) {
	qcsynd_t synd = malloc(sizeof (struct qcsynd));

	synd->length = length;
	synd->weight = 0;
	synd->coeff = (char *) calloc(length, sizeof (char));

	return synd;
}

void qcsynd_free(qcsynd_t synd) {
	free(synd->coeff);
	free(synd);
}

void qcsynd_copy(qcsynd_t copy, qcsynd_t s) {
	copy->weight = s->weight;
	memcpy(copy->coeff, s->coeff, s->length * sizeof (char));
}

qcsynd_t qcsynd_copy_alloc(qcsynd_t s) {
	qcsynd_t copy = qcsynd_new(s->length);
	copy->weight = s->weight;
	memcpy(copy->coeff, s->coeff, s->length * sizeof (char));
	return copy;
}


spectre_t spectre_new(int length) {
	spectre_t s = malloc(sizeof (struct spectre));

	int l = (length)/2+1;
	s->length = l;
	s->coeff = (char *) calloc(l, sizeof (char));

	return s;
}

void spectre_free(spectre_t s) {
	free(s->coeff);
	free(s);
}

void make_spectre_of(spectre_t s, qcblock_t e) {
  for (int i=0; i<spectre_length(s); ++i) 
    {
      spectre_set_coeff(s,i,0);
    } 
  
  int p = BLOCK_LENGTH;
  int w = qcblock_weight(e);
  int i, j, d1, d2, d;
  
  for (i=0; (i<w-1) & (qcblock_index(e,i) < BLOCK_LENGTH); ++i) {
    for (j=i+1; (j<w) & (qcblock_index(e,j) < BLOCK_LENGTH); ++j) {
      d1 = qcblock_index(e,j) - qcblock_index(e,i);
      d2 = p - d1;
      d = d1>=d2?d2:d1 ;		
      spectre_set_coeff(s,d,1);
    }
  }
}



/* allocate a new qcblock_t to contain the t indices in v[]. */
qcblock_t qcblock_from_indexarray(index_t * v, int length, int weight) {
	qcblock_t h = qcblock_new(length, weight);

	memcpy(h->index, v, weight * sizeof (index_t));

	return h;
}

/* compute the xor of two blocks in a third one, previously allocated */
qcblock_t qcblock_xor_noalloc(qcblock_t h, qcblock_t h0, qcblock_t h1) {
	int i, j;

	/* make sure we have enough space in the block h */
	h->index = realloc(h->index, (qcblock_weight(h0) + qcblock_weight(h1)) * sizeof (index_t));
	i = 0;
	j = 0;
	while (1) {
		if (j == qcblock_weight(h1)) {
			for (; i < qcblock_weight(h0); ++i) {
				h->index[h->weight] = qcblock_index(h0, i);
				h->weight++;
			}
			break;
		} else if (i == qcblock_weight(h0)) {
			for (; j < qcblock_weight(h1); ++j) {
				h->index[h->weight] = qcblock_index(h1, j);
				h->weight++;
			}
			break;
		} else if (qcblock_index(h0, i) == qcblock_index(h1, j)) {
			++i;
			++j;
		} else if (qcblock_index(h0, i) > qcblock_index(h1, j)) {
			h->index[h->weight] = qcblock_index(h1, j);
			h->weight++;
			++j;
		} else { // qcblock_index(h0, i) < qcblock_index(h1, j)
			h->index[h->weight] = qcblock_index(h0, i);
			h->weight++;
			++i;
		}
	}
	/* reajust the allocated size */
	h->index = realloc(h->index, h->weight * sizeof (index_t));

	return h;
}
/* compute the xor of two blocks return the result in a newly
	 allocated block */
qcblock_t qcblock_xor(qcblock_t h0, qcblock_t h1) {
	return qcblock_xor_noalloc(qcblock_new(h0->length, 0), h0, h1);
}

static int indexcomp(const void *i, const void *j) {
	return (int) (*(index_t *)i - *(index_t *)j);
}

/* pick a random (sparse) binary block h of length h->length and of
	 weight h->weight in a previously allocated block */
qcblock_t qcblock_rand_noalloc(qcblock_t h, int (*rand_fct)()) {
	int i, j;
	index_t x, rand_perm_block[h->length];

	for (i = 0; i < h->length; ++i) {
		rand_perm_block[i] = i;
	}

	for (i = 0; i < h->weight; ++i) {
		j = i + rand_fct() % (h->length - i);
		x = rand_perm_block[i];
		rand_perm_block[i] = rand_perm_block[j];
		rand_perm_block[j] = x;
	}

	qsort(rand_perm_block, h->weight, sizeof (index_t), indexcomp);
	memcpy(h->index, rand_perm_block, h->weight * sizeof (index_t));

	return h;
}

/* allocate and pick a random (sparse) binary block of given length
	 and weight */
qcblock_t qcblock_rand(int length, int weight, int (*rand_fct)()) {
	return qcblock_rand_noalloc(qcblock_new(length, weight), rand_fct);
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

	for (l = 0; l < qcblock_weight(h); ++l) {
		i = j + qcblock_index(h, l);
		if (i >= qcblock_length(h))
			i -= qcblock_length(h);
		c = qcsynd_flip_coeff(synd, i);
		synd->weight += 2 * c - 1;
	}

	return synd;
}

/* add to the syndrome synd the syndrome of the error e in the QC-MDPC
	 code H */
qcsynd_t qcmdpc_synd_adjust(qcsynd_t synd, qcmdpc_t H, qcblock_t e) {
	int i, j, l, p, offset;
	qcblock_t h;

	h = qcmdpc_block(H, 0);
	p = qcblock_length(h);
	for (offset = 0, i = 0, l = 0; i < qcblock_weight(e); ++i) {
		j = qcblock_index(e, i) - offset;
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
counter_t qcmdpc_counter(qcmdpc_t H, qcsynd_t synd, int j) {
	int i, l, p;
	counter_t w;
	qcblock_t h;

	w = 0;
	p = qcmdpc_block_length(H);
	h = qcmdpc_block(H, j / p);
	j %= p;
	for (l = 0; l < p; ++l) {
		i = j + qcblock_index(h, l);
		if (i >= p)
			i -= p;
		w += qcsynd_coeff(synd, i);
	}

	return w;
}

/* computes all counter values, returns an array of counters of size
	 codelength */
counter_t * qcmdpc_count(qcmdpc_t H, qcsynd_t synd) {
	int i, j, l, u, p;
	counter_t * counter, * temp;
	qcblock_t h;

	p = qcmdpc_block_length(H);
	counter = (counter_t *) calloc(INDEX * p, sizeof (counter_t));

	for (i = 0; i < p; ++i) {
		if (qcsynd_coeff(synd, i)) {
			for (temp = counter, u = 0; u < INDEX; ++u, temp += p) {
				h = qcmdpc_block(H, u);
				for (l = 0; l < qcblock_weight(h); ++l) {
					j = i - qcblock_index(h, l);
					if (j < 0)
						j += p;
					temp[j]++;
				}
			}
		}
	}

	return counter;
}
