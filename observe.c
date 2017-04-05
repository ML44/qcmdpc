# include "observe.h"

// -----------------------------------
// For some code h, for each possible distance in the spectrum,
// computes the averaged weight of syndroms of errors containing
// this distance, over a sample of N errors
// -----------------------------------
void spectrum_reconstruction(int p, int w, int t, int N, int se, int sh) {
  qcblock_t e = qcblock_new(0,0);
  qcsynd_t synd_e = qcsynd_new(0), spectre_e = qcsynd_new(0);
  int sw;

  // Creation of the code
  mysrnd(sh);
  qcmdpc_t H = qcmdpc_rand(p, w, myrnd);

  // Creation of two counters
  dist_count_t * dist_freq_counter = dist_count_new(p/2);
  dist_count_t * sweight_counter = dist_count_new(p/2);
  
  
  for (int i=0; i<N; ++i) {
    // Generate an error
    mysrnd(se);
    e = qcblock_rand(p, t, myrnd);
    
    // Compute the syndrom and its weight
    synd_e = qcmdpc_synd(H, e);
    sw = vect_weight(synd_e);
 
    // Compute the spectrum of e
    spectre_e = spectrum(e);

    // Add the weight to the values of the counters that are in the spectrum
    spectrum_add_to_counters(sweight_counter, dist_freq_counter, spectre_e, sw);

    // Update seed
    se++;
  }

  // compute the ratios
  dist_count_float_t * ratio_counter = dist_count_float_new(p/2);
  for (int i=0; i<p/2; i++) {
    if (dist_freq_counter[i]>0) {
      ratio_counter[i] = sweight_counter[i];
      ratio_counter[i] /= dist_freq_counter[i];
    }
  }

  // compute the spectrum of h
  // is only used to check the correctness (here to give colors to the points)
  qcsynd_t spectrum_h = spectrum(qcmdpc_block(H,0));

  // write .dat file
   FILE *fp;
   fp = fopen("./dat/1.dat", "w+");
   fprintf(fp, "#Nombre d'essais = %d, \n#Poids de l'erreur = %d, \n#Taille d'un bloc = %d, \n#Poids de h = %d. \n", N, p, t, w);
   for (int i=0; i<p/2; i++) {
     fprintf(fp, "%d \t %f \t %d \n", i, ratio_counter[i], vect_coeff(spectrum_h,i));
   }
   fclose(fp);  

  // call gnuplot
  system("gnuplot gnuplot-instructions.gnu > ./dat/1.png");
}


















qcsynd_t observe_spectrum(qcmdpc_t H, int p, int w, int t, int N, int se, float threshold) {
  qcblock_t e = qcblock_new(0,0);
  qcsynd_t synd_e = qcsynd_new(0), spectre_e = qcsynd_new(0);
  int sw;

  
  // Creation of two counters
  dist_count_t * dist_freq_counter = dist_count_new(p/2);
  dist_count_t * sweight_counter = dist_count_new(p/2);
  
  
  for (int i=0; i<N; ++i) {
    // Generate an error
    mysrnd(se);
    e = qcblock_rand(p, t, myrnd);
    
    // Compute the syndrom and its weight
    synd_e = qcmdpc_synd(H, e);
    sw = vect_weight(synd_e);
 
    // Compute the spectrum of e
    spectre_e = spectrum(e);

    // Add the weight to the values of the counters that are in the spectrum
    spectrum_add_to_counters(sweight_counter, dist_freq_counter, spectre_e, sw);

    // Update seed
    se++;
  }

  // compute the ratios
  dist_count_float_t * ratio_counter = dist_count_float_new(p/2);
  for (int i=0; i<p/2; i++) {
    if (dist_freq_counter[i]>0) {
      ratio_counter[i] = sweight_counter[i];
      ratio_counter[i] /= dist_freq_counter[i];
    }
  }

  // compute the (hypothetical) spectrum of h
  qcsynd_t spectrum_h = spectrum_from_counter(ratio_counter, p/2, threshold);

  // write .dat file
   FILE *fp;
   fp = fopen("./dat/1.dat", "w+");
   fprintf(fp, "#Nombre d'essais = %d, \n#Poids de l'erreur = %d, \n#Taille d'un bloc = %d, \n#Poids de h = %d. \n", N, p, t, w);
   for (int i=0; i<p/2; i++) {
     fprintf(fp, "%d \t %f \t %d \n", i, ratio_counter[i], vect_coeff(spectrum_h,i));
   }
   fclose(fp);  

  // call gnuplot
  system("gnuplot gnuplot-instructions.gnu > ./dat/1.png");

  return spectrum_h;
}





















// ********************************************************
// ********************************************************

// Data functions

// ********************************************************
// ********************************************************




// -----------------------------------
// Computes the syndrom weight distribution of a sample of N errors for a fixed
// code h containing the distance d.
// Two different cases : whether or not e contains distance d.
// Generates the general distribution and the two subset distributions.
// -----------------------------------
void syndrom_weight_distribution(int p, int wh, int we, int d, int N, int se, int sH) { 
  qcmdpc_t H = qcmdpc_new(); 
  qcblock_t e = qcblock_new(0,0) ;
  qcsynd_t s = qcsynd_new(0);
  int ws = 0;
  int N0 = 0, N1 = 0;
  
  // Creation of the code
  do {
  mysrnd(sH);
  H = qcmdpc_rand(p, wh, myrnd);
  sH++;
  } while (!(vect_coeff(spectrum(qcmdpc_block(H,0)),d)));
  /* } while (!(is_in_dist(d,qcmdpc_block(H,0)))); */

  dist_count_t * weight_counter_0 = dist_count_new(p);
  dist_count_t * weight_counter_1 = dist_count_new(p);

  for (int i=0; i<N; i++) {

    // Creation of the error
    mysrnd(se);
    e = qcblock_rand(p, we, myrnd);
    se++;
    
    // Computation of the syndrom weight
    s = qcmdpc_synd(H, e);
    ws = vect_weight(s);
    
    if (vect_coeff(spectrum(e),d)) {
      N1++;
      weight_counter_1[ws]+=1; 
    } 
    else {
      N0++;
      weight_counter_0[ws]+=1; 
    } 
  }


   FILE *fp;
   fp = fopen("./dat/2.dat", "w+");
   fprintf(fp, "#Nombre d'essais = %d, \n#Erreur ne contenant pas d (N0) = %d, \n#Erreur contenant d (N1) = %d, \n#Longueur du vecteur = %d, \n#Poids de l'erreur = %d, \n#Poids de h = %d. \n", N, N0, N1, p, we, wh);

   for (int i=0; i<p/2; i++) {
     fprintf(fp, "%d \t %f \t %f \n", i, (float) weight_counter_0[2*i] / N, (float) weight_counter_1[2*i] / N);
   }
   fclose(fp);  

   fp = fopen("./dat/3_0.dat", "w+");
   fprintf(fp, "#Nombre d'essais = %d, \n#Erreur ne contenant pas d (N0) = %d, \n#Erreur contenant d (N1) = %d, \n#Longueur du vecteur = %d, \n#Poids de l'erreur = %d, \n#Poids de h = %d. \n", N, N0, N1, p, we, wh);

   for (int i=0; i<p/2; i++) {
     for (int k=0; k<weight_counter_0[2*i]; k++) {
       fprintf(fp, "%d \n", i);
     }
   }
   fclose(fp);  


   fp = fopen("./dat/3_1.dat", "w+");
   fprintf(fp, "#Nombre d'essais = %d, \n#Erreur ne contenant pas d (N0) = %d, \n#Erreur contenant d (N1) = %d, \n#Longueur du vecteur = %d, \n#Poids de l'erreur = %d, \n#Poids de h = %d. \n", N, N0, N1, p, we, wh);

   for (int i=0; i<p/2; i++) {
     for (int k=0; k<weight_counter_1[2*i]; k++) {
       fprintf(fp, "%d \n", i);
     }
   }
   fclose(fp);  


  qcmdpc_free(H);
  qcblock_free(e);
  qcsynd_free(s);
  dist_count_free(weight_counter_0);
  dist_count_free(weight_counter_1);
  system("gnuplot gnuplot-instructions2.gnu > ./dat/2.png");
}
