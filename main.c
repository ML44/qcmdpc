#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "lib.h"

#include "observe.h"
#include "reconstruct.h"


int main(int argc, char ** argv) {

  

  // arguments
  int arg_count, N, p, t, se, sh, w, n0, n, d;
  arg_count = 0;
  // nombre de tests
  N = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1000;
  // seed for H
  sh = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
  // seed for e
  se = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
  // distance d
  d = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
  // longueur du bloc
  p = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 4801;  
  // poids du polynome h
  w = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 45; 
  // nombre d'erreurs (poids de e)
  t = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 42;

  
  

  
  mysrnd(sh);
  qcblock_t h = qcblock_rand(p,w,myrnd);
  /* qcblock_print(h, "h"); */
  /* printf("%d\n",vect_length(h)); */

  qcsynd_t s = spectrum(h);
  /* qcsynd_print(s, "s"); */
  
  list_t A = construct_A(s, get_p1(s));
  printf("p1 = %d\n",get_p1(s));
  list_print(A,"A = ");
  
  list_list_t E = dsr(s,w);
  printf("n = %d\n",list_list_length(E));
  /* list_list_print(E, "E"); */
  


  /* // Creation of the code */
  /* mysrnd(sh); */
  /* qcmdpc_t H = qcmdpc_rand(p, w, myrnd); */

  /* // Observe the spectrum */
  /* float threshold = 1323.7; // a fixer comment ? */
  /* qcsynd_t spectrum = observe_spectrum(H, p, w, t, N, se, threshold); */
  /* qcsynd_print(spectrum,"observed"); */
  /* printf("%d\n", spectrum->weight); */

  
  
  /* list_list_t candidates = dsr(spectrum, w); */
  /* list_list_print(candidates, ""); */
  



  /* mysrnd(sh); */
  /* qcblock_t h = qcblock_rand(p,w,myrnd); */
  /* qcblock_print(h, "h"); */
  /* qcsynd_t s = spectrum(h); */
  /* qcsynd_print(s, "s"); */

  /* for(int i=0; i<vect_length(s);i++)  */
  /*   { */
  /*     if (i%3==0)  */
  /* 	{ */
  /* 	  vect_coeff(s,i)=0; */
	  
  /* 	} */
  /*   } */
  /* qcsynd_print(s, "s'"); */
  

  /* graph_t g = graph_new(h, s); */
  /* graph_print(g); */
  /* list_list_t l = graph_findMaxClique(g); */
  /* list_list_print(l, "result"); */
  

  /* syndrom_weight_distribution(p, w, t, d, N, se, sh); */
  /* block_reconstruction(p, w, sh); */
  /* qcsynd_t spectrum = observe_spectrum(H, p, w, t, N, se, threshold); */




  /* int i; */
  /* clock_t time_begin; */
  /* clock_t time_end; */
  /* int time_spent; */

  /* for (i=0;i<N;++i) { */
  /*   printf("\nTEST %d : \n",i); */
  /*   time_begin = clock();   */
  /*   block_reconstruction(p, w, i+1); */
  /*   time_end = clock(); */
  /*   time_spent = (1000*(time_end - time_begin)) / CLOCKS_PER_SEC; */
  /*   printf("time = %d ms \n", time_spent); */
  /* } */

  
  return 0;
}
