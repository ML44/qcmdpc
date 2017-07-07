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
  qcmdpc_t H = qcmdpc_rand(p,w,myrnd);
  
  qcblock_t e1 = qcblock_new(p,t);
  qcblock_t e2 = qcblock_new(p,t);
  qcsynd_t synd_e1;
  qcsynd_t synd_e2;
  qcsynd_t synd_e;
  
  qcsynd_t spectre_e1 = qcsynd_new(0);
  qcsynd_t spectre_e2 = qcsynd_new(0);
  int sw;

  
  // Creation of two counters
  dist_count_t * dist_freq_counter1 = dist_count_new(p/2);
  dist_count_t * sweight_counter1 = dist_count_new(p/2);
  dist_count_t * dist_freq_counter2 = dist_count_new(p/2);
  dist_count_t * sweight_counter2 = dist_count_new(p/2);
  //  dist_count_t * dist_freq_counter0 = dist_count_new(p);
  // dist_count_t * sweight_counter0 = dist_count_new(p);
  
  
  for (int i=0; i<N; ++i) {
    // Generate an error
    mysrnd(se);
    e1 = qcblock_rand_noalloc(e1, myrnd);
    se++;
    mysrnd(se);
    e2 = qcblock_rand_noalloc(e2, myrnd);
    se++;
    
    // Compute the syndrom and its weight    
    synd_e1 = qcmdpc_synd(qcmdpc_block(H,0), e1);
    synd_e2 = qcmdpc_synd(qcmdpc_block(H,1), e2);
    
    
    // Sum the syndroms
    synd_e = qcsynd_xor(synd_e1, synd_e2);
    sw = vect_weight(synd_e);
    free(synd_e);
    free(synd_e1);
    free(synd_e2);
    
 
    // Compute the spectrum of e
    spectre_e1 = spectrum(e1);
    spectre_e2 = spectrum(e2);

    // Add the weight to the values of the counters that are in the spectrum
    spectrum_add_to_counters(sweight_counter1, dist_freq_counter1, spectre_e1, sw);
    spectrum_add_to_counters(sweight_counter2, dist_freq_counter2, spectre_e2, sw);

    free(spectre_e1);
    free(spectre_e2);
    //  free(sw);
    

  }
  
  // compute the ratios
  dist_count_float_t * ratio_counter1 = dist_count_float_new(p/2);
  for (int i=0; i<p/2; i++) {
    if (dist_freq_counter1[i]>0) {
      ratio_counter1[i] = sweight_counter1[i];
      ratio_counter1[i] /= dist_freq_counter1[i];
    }
  }
  dist_count_float_t * ratio_counter2 = dist_count_float_new(p/2);
  for (int i=0; i<p/2; i++) {
    if (dist_freq_counter1[i]>0) {
      ratio_counter2[i] = sweight_counter2[i];
      ratio_counter2[i] /= dist_freq_counter2[i];
    }

    
  }

  // compute the spectrum of h
  // is only used to check the correctness (here to give colors to the points)
  qcsynd_t spectrum_h1 = spectrum(qcmdpc_block(H,0));
  qcsynd_t spectrum_h2 = spectrum(qcmdpc_block(H,1));

  // write .dat file
   FILE *fp;
   fp = fopen("./dat/8-1.dat", "w+");
   fprintf(fp, "#Nombre d'essais = %d, \n#Poids de l'erreur = %d, \n#Taille d'un bloc = %d, \n#Poids de h = %d. \n", N, p, t, w);
   for (int i=0; i<p/2; i++) {
     fprintf(fp, "%d \t %f \t %d \n", i, ratio_counter1[i], vect_coeff(spectrum_h1,i));
   }
   fclose(fp);  
   fp = fopen("./dat/8-2.dat", "w+");
   fprintf(fp, "#Nombre d'essais = %d, \n#Poids de l'erreur = %d, \n#Taille d'un bloc = %d, \n#Poids de h = %d. \n", N, p, t, w);
   for (int i=0; i<p/2; i++) {
     fprintf(fp, "%d \t %f \t %d \n", i, ratio_counter2[i], vect_coeff(spectrum_h2,i));
   }
   fclose(fp);  

  // call gnuplot
   float s = 1918;
   FILE *fpsh;
   fpsh = fopen("./dat/8-1.sh", "w+");
   fprintf(fpsh, "gnuplot -e \"s = %f; mytitle = 'Average syndrom weight per distance (1 block, %d tries)'\" gnuplot-instructions8-1.gnu > ./dat/8-1.png", s, N);
   fclose(fpsh);  
   system("sh ./dat/8-1.sh");
   
   fpsh = fopen("./dat/8-2.sh", "w+");
   fprintf(fpsh, "gnuplot -e \"s = %f; s0 = %f; s1 = %f; s2 = %f; s3 = %f; s4 = %f; mytitle = 'Average syndrom weight per distance (1 block, %d tries)'\" gnuplot-instructions8-2.gnu > ./dat/8-2.png", s, sigma(0,p,w,t), sigma(1,p,w,t), sigma(2,p,w,t), sigma(3,p,w,t), sigma(4,p,w,t), N);
   fclose(fpsh);  
   system("sh ./dat/8-2.sh");

  


  


  /* ********************************************************************* */  




  
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

  /* spectrum_reconstruction(p, w, t, N, se, sh); */
  
  






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
