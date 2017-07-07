#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "qcmdpc_dec.h"

long random(void);
void srandom(unsigned int seed);
int myrnd() { return random(); }
void mysrnd(int seed) { srandom(seed); }

int main(int argc, char ** argv) {
	int t, se, sH, arg_count, r, i;
	qcmdpc_t H;
	qcblock_t e;
	int stat_iter[MAX_ITER + 1];

	arg_count = 0;
	t = atoi(argv[++arg_count]);
	r = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
	sH = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
	se = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;

	mysrnd(sH);
	H = qcmdpc_rand(BLOCK_LENGTH, BLOCK_WEIGHT, myrnd);

	for (i = 0; i <= MAX_ITER; ++i) {
		stat_iter[i] = 0;
	}

	while (r--) {
#ifdef VERBOSE
		printf("#%d,%d %d %d %d ", sH, se, BLOCK_LENGTH, BLOCK_WEIGHT, t);
#endif
		mysrnd(se);
		e = qcblock_rand(INDEX * BLOCK_LENGTH, t, myrnd);
		i = qcmdpc_decode(H, e);
		stat_iter[i]++;

		qcblock_free(e);

#ifdef VERBOSE
		printf("\n");
#endif

		se++;
	}
	qcmdpc_free(H);

#ifndef VERBOSE
	for (i = 1; i <= MAX_ITER; ++i) {
		if (stat_iter[i] != 0) {
			printf("%d iterations:\t %d\n", i, stat_iter[i]);
		}
	}
	printf("failures (%d):\t %d\n", MAX_ITER, stat_iter[0]);
#endif


	return 0;
}
