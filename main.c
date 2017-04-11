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

  
  

  /* mysrnd(sh); */
  /* qcblock_t h = qcblock_rand(p,w,myrnd); */
  /* /\* qcblock_print(h, "h"); *\/ */
  /* /\* printf("%d\n",vect_length(h)); *\/ */

  /* qcsynd_t s = spectrum(h); */
  /* /\* qcsynd_print(s, "s"); *\/ */
  
  /* list_t A = construct_A(s, get_p1(s)); */
  /* printf("p1 = %d\n",get_p1(s)); */
  /* list_print(A,"A = "); */
  
  /* list_list_t F = dsr(s,w); */
  /* printf("lF = %d\n",list_list_length(F)); */
  /* list_list_print(F, "F"); */

  spectrum_reconstruction(p, w, t, N, se, sh);
  






  /* ********************************************************************* */  


  /* // Creation of the code */
  /* mysrnd(sh); */
  /* qcmdpc_t H = qcmdpc_rand(p, w, myrnd); */

  /* // Observe the spectrum */
  /* float threshold = 138.5; // a fixer comment ? */
  /* qcsynd_t spectrum = observe_spectrum(H, p, w, t, N, se, threshold); */
  /* qcblock_print(H, "h"); */
  /* /\* qcsynd_print(spectrum,"observed"); *\/ */
  /* /\* printf("%d\n", spectrum->weight); *\/ */
  /* /\* printf("%d\n", spectrum->length); *\/ */

  /* list_list_t candidates = dsr(spectrum, w); */
  /* list_list_print(candidates, ""); */

  
  return 0;
}
