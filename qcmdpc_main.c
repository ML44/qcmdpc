
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "qcmdpc_dec.h"
#include "qcmdpc_attack.h"


int main(int argc, char ** argv) {

  

  // arguments
  int arg_count, N, p, t, se, sH, bl, bw;
  arg_count = 0;
  // nombre de tests
  N = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1000;
  // longueur du bloc
  p = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 4801;
  bl = p;
  // poids du polynome h (d)
  bw = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 45; 
  // nombre d'erreurs (poids de e)
  t = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 42; 
  // seed for e
  se = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
  // seed for H
  sH = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;

  test_spectrum_reconstruction(p, bl, bw, t, N, se, sH);
  


/*
  // arguments
  int arg_count, len, d, seed;
  arg_count = 0;
  // longueur du bloc
  len = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 10;
  // poids du polynome h (d)
  d = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 4; 
  // random seed
  seed = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;

  int i;
  clock_t time_begin;
  clock_t time_end;
  int time_spent;

  for (i=0;i<seed;++i) {
    printf("TEST %d : \n",i);
    time_begin = clock();
    test_block_reconstruction(len, d, i);
    time_end = clock();
    time_spent = (1000*(time_end - time_begin)) / CLOCKS_PER_SEC;
    printf("time = %d ms \n", time_spent);
  }
*/
  
  return 0;
}
