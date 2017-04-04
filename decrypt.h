#include "qcmdpc.h"

#ifndef BLOCK_LENGTH
#define BLOCK_LENGTH 4801
#endif

#ifndef BLOCK_WEIGHT
#define BLOCK_WEIGHT 45
#endif

#ifndef MAX_ITER
#define MAX_ITER 10
#endif

int qcmdpc_decode(qcmdpc_t H, qcblock_t e);
