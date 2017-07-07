#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "qcmdpc_dec.h"
#include "misc.h"

qcblock_t flip_list(counter_t * counter, int threshold) {
	int i, t;
	index_t res[INDEX * BLOCK_LENGTH];
	qcblock_t e_flip;

	for (t = 0, i = 0; i < INDEX * BLOCK_LENGTH; ++i) {
		if (counter[i] >= threshold) {
			res[t] = i;
			t++;
		}
	}

	e_flip = qcblock_from_indexarray(res, INDEX * BLOCK_LENGTH, t);

	return e_flip;
}

qcblock_t * flip_list_range(counter_t * counter, int threshold_min, int threshold_max) {
	int i, j, delta;
	qcblock_t * e_flip;

	delta = threshold_max - threshold_min + 1;
	index_t t[delta], res[delta][INDEX * BLOCK_LENGTH];

	memset(t, 0, delta * sizeof (index_t));
	for (j = 0; j < INDEX * BLOCK_LENGTH; ++j) {
		if (counter[j] >= threshold_min) {
			i = threshold_max - counter[j];
			if (i < 0)
				i = 0;
			res[i][t[i]] = j;
			t[i]++;
		}
	}

	e_flip = (qcblock_t *) malloc(delta * sizeof (qcblock_t));
	for (i = 0; i < delta; ++i) {
		e_flip[i] = qcblock_from_indexarray(res[i], INDEX * BLOCK_LENGTH, t[i]);
	}

	return e_flip;
}

/* side effect: update the syndrome synd */
qcblock_t qcmdpc_iter_range(qcmdpc_t H, qcblock_t e, qcsynd_t synd, int iter) {
	int threshold, threshold_min, threshold_max, delta, i;
	counter_t * counter;
	qcblock_t * e_flip_range, e_flip, e_new;
	qcsynd_t s_new;

	if (qcsynd_weight(synd) == 0)
		return qcblock_new(INDEX * BLOCK_LENGTH, 0); // error of weight 0

	counter = qcmdpc_count(H, synd);

	threshold_max = nofail_threshold(BLOCK_LENGTH, // block length
																	 BLOCK_WEIGHT, // block weight
																	 qcsynd_weight(synd)); // syndrome weight

	threshold_min = (qcsynd_weight(synd) * BLOCK_WEIGHT) / BLOCK_LENGTH;

	delta = threshold_max - threshold_min + 1;
	e_flip_range = flip_list_range(counter, threshold_min, threshold_max);
	s_new = qcsynd_copy_alloc(synd);
	e_flip = qcblock_new(INDEX * BLOCK_LENGTH, 0);
	for (i = 0; i < delta; ++i) {
		if (qcblock_weight(e_flip_range[i]) == 0)
			continue;
		qcmdpc_synd_adjust(s_new, H, e_flip_range[i]);
		if (qcsynd_weight(s_new) >= qcsynd_weight(synd)) {
			threshold = threshold_max - i + 1;
			break;
		}
		qcsynd_copy(synd, s_new);
		e_new = qcblock_xor(e_flip, e_flip_range[i]);
		qcblock_free(e_flip);
		e_flip = e_new;
	}
	e_new = qcblock_xor(e, e_flip);

#ifdef VERBOSE
	printf("%d %d %d %d ", threshold, qcblock_weight(e_flip), qcblock_weight(e_new), qcsynd_weight(synd));
#endif

	for (i = 0; i < delta; ++i) {
		qcblock_free(e_flip_range[i]);
	}
	free(e_flip_range);
	qcblock_free(e_flip);
	qcsynd_free(s_new);
	free(counter);

	return e_new;
}

/* side effect: update the syndrome synd */
qcblock_t qcmdpc_iter(qcmdpc_t H, qcblock_t e, qcsynd_t synd, int iter) {
	int threshold;
	counter_t * counter;
	qcblock_t e_flip, e_new;

	if (qcsynd_weight(synd) == 0)
		return qcblock_new(INDEX * BLOCK_LENGTH, 0); // error of weight 0

	counter = qcmdpc_count(H, synd);

	/* cheat: we should not be able to choose the threshold from t */
	threshold = best_threshold_error(INDEX * BLOCK_LENGTH, // code length
																	 INDEX * BLOCK_WEIGHT, // row weight
																	 qcsynd_weight(synd), // syndrome weight
																	 qcblock_weight(e)); // error weight

	e_flip = flip_list(counter, threshold);
	qcmdpc_synd_adjust(synd, H, e_flip);
	e_new = qcblock_xor(e, e_flip);

#ifdef VERBOSE
		printf("%d %d %d %d ", threshold, qcblock_weight(e_flip), qcblock_weight(e_new), qcsynd_weight(synd));
#endif

	free(counter);
	qcblock_free(e_flip);

	return e_new;
}

/* returns the number of iterations, 0 in case of failure */
int qcmdpc_decode(qcmdpc_t H, qcblock_t e) {
	int i, iter = 0;
	qcblock_t * error;
	qcsynd_t synd;

	error = (qcblock_t *) calloc(MAX_ITER + 1, sizeof (qcblock_t));
	error[0] = e;
	synd = qcmdpc_synd(H, e);
#ifdef VERBOSE
	printf("%d ", qcsynd_weight(synd));
#endif
	for (iter = 0; (iter < MAX_ITER) && (qcsynd_weight(synd) > 0); ++iter) {
		error[iter + 1] = qcmdpc_iter(H, error[iter], synd, iter);
	}
#ifdef VERBOSE
	printf("%d ", iter);
#endif

	for (i = 1; i <= iter; ++i) {
		qcblock_free(error[i]);
	}
	free(error);

	if (qcsynd_weight(synd) > 0)
		iter = 0;
	qcsynd_free(synd);

	return iter;
}
