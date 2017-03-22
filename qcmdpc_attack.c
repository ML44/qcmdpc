#include "qcmdpc_attack.h"

// ********************************************************
// ********************************************************

// Random 

// ********************************************************
// ********************************************************


// -----------------------------------
// new random generator
// -----------------------------------
int myrnd() { return random(); }


// -----------------------------------
// feeds random generator with seed
// -----------------------------------
void mysrnd(int seed) { srandom(seed); }





// ********************************************************
// ********************************************************

// Qcsynd

// ********************************************************
// ********************************************************



// -----------------------------------
// prints the qcsynd with str prefix message
// -----------------------------------
void qcsynd_print(qcsynd_t s, char * str) {
  int i;
  printf("%s = { ", str);
  for (i=0; i<qcsynd_length(s); i++) {
    printf("%d ; ", qcsynd_coeff(s, i));
  }
  printf("} \n");
}



// -----------------------------------
// prints the l first values of two qcsynd side by side
// -----------------------------------
void qcsynd_print2(qcsynd_t s1, qcsynd_t s2, int l) {
  int i;
  int s = 0;
  for (i=0; i<l; i++) {
    printf("%d - %d\n", qcsynd_coeff(s1, i), qcsynd_coeff(s2, i));
    s+= 1 - (qcsynd_coeff(s1, i) ^ qcsynd_coeff(s2, i));
  }
  printf("similarite : %d / %d\n", s, l);
}


// -----------------------------------
// compares the first l values of two qcsynd
// -----------------------------------
void qcsynd_compare(qcsynd_t s1, qcsynd_t s2, int l) {
  int i;
  int c00=0, c01=0, c10=0, c11=0;
  for (i=0; i<l; i++) {
    if(qcsynd_coeff(s1, i)) {
      if(qcsynd_coeff(s2, i)) {
	c11+=1;
      }
      else{
	c10+=1;
      }
    }
    else {
      if(qcsynd_coeff(s2, i)) {
	c01+=1;
      }
      else{
	c00+=1;
      }
    }
  }
  printf("0 - 0 : %d\n", c00);
  printf("1 - 1 : %d\n", c11);
  printf("1 - 0 : %d\n", c10);
  printf("0 - 1 : %d\n", c01);
  printf("similarite : %d / %d\n", c00+c11, l);
}




// -----------------------------------
// sets to 1 the ith coef of s
// -----------------------------------
void qcsynd_set_coeff(qcsynd_t s, int i) {
  if (!(qcsynd_coeff(s,i))) {
    (s)->coeff[i]=1;
    (s)->weight+=1;
  }
}


// -----------------------------------
// checks if s1 is included in s2
// -----------------------------------
char qcsynd_inclusion(qcsynd_t s1, qcsynd_t s2) {
  if (qcsynd_length(s1)!=qcsynd_length(s2)) {
    return 0;
  }
  for (int i=0; i<qcsynd_length(s1);++i) {
    if (qcsynd_coeff(s1,i)>qcsynd_coeff(s2,i)) {
      return 0;
    }
  }
  return 1;
}









// -----------------------------------
// -----------------------------------
// Qcblock
// -----------------------------------
// -----------------------------------


// -----------------------------------
// prints the values of the qcblock with str prefix message
// -----------------------------------
void qcblock_print(qcblock_t h, char * str) {
  int i;
  printf("%s = ", str);
  for (i=0; i<qcblock_weight(h); ++i) {
    printf("%d ; ", qcblock_index(h, i));
  }
  printf("\n");
}







// ********************************************************
// ********************************************************

// Distance Counter

// ********************************************************
// ********************************************************



// -----------------------------------
// alocates a counter of given length
// -----------------------------------
dist_count_t * dist_count_new(int length) {
	dist_count_t * counter;
	counter = (dist_count_t *) calloc(length, sizeof (dist_count_t));
	return counter;
}




// -----------------------------------
// prints the values of the counter with str prefix message
// -----------------------------------
void dist_count_print(dist_count_t * counter, int p, char * str) {
  int i;
  printf("%s = { ", str);
  for (i=0; i<p; i++) {
    printf("%d ; ", counter[i]);
  }
  printf("} \n");
}




// -----------------------------------
// frees the counter
// -----------------------------------
void dist_count_free(dist_count_t * counter) {
	free(counter);
}




// -----------------------------------
// computes the mean of the counter
// -----------------------------------
int dist_count_mean(dist_count_t * counter, int p) {
  unsigned long int m = 0;
  int i;
  for (i=0; i<p; ++i) {
    m += counter[i];
  }
  return m/p;
}




// ********************************************************
// ********************************************************

// Distance Counter Float

// ********************************************************
// ********************************************************



// -----------------------------------
// alocates a counter of given length
// -----------------------------------
dist_count_float_t * dist_count_float_new(int length) {
	dist_count_float_t * counter;
	counter = (dist_count_float_t *) calloc(length, sizeof (dist_count_float_t));
	return counter;
}




// -----------------------------------
// prints the values of the counter with str prefix message
// -----------------------------------
void dist_count_float_print(dist_count_float_t * counter, int p, char * str) {
  int i;
  printf("%s = { ", str);
  for (i=0; i<p; i++) {
    printf("%f ; ", counter[i]);
  }
  printf("} \n");
}




// -----------------------------------
// frees the counter
// -----------------------------------
void dist_count_float_free(dist_count_float_t * counter) {
	free(counter);
}
















// ********************************************************
// ********************************************************

// Qclist

// ********************************************************
// ********************************************************



// -----------------------------------
// new empty qclist
// -----------------------------------
qclist_t qclist_init(int len) {
  qclist_t l = malloc( sizeof ( struct qclist ) );
  l->index = NULL;
  l->weight=0;
  l->length=len;
  return l;
}



// -----------------------------------
// tests if qclist is empty
// -----------------------------------
char qclist_isempty(qclist_t l) {
  if (l->index == NULL) {return 1;} return 0;
}



// -----------------------------------
// adds value v at the beginning of the qclist
// -----------------------------------
void qclist_add(qclist_t l, index_t v) {
  node_t head = malloc(sizeof(node_t));
  head->val = v;
  head->next = l->index;
  l->index = head;
  l->weight++;
}



// -----------------------------------
// removes the first value of the qclist
// -----------------------------------
void qclist_remove(qclist_t l) {
  if(l->weight>0) {
    l->index = l->index->next;
    l->weight--;
  }
}



// -----------------------------------
// prints the qclist with str prefix message
// -----------------------------------
void qclist_print(qclist_t l, char * str) {
  printf("%s = { ", str);
  node_t current = l->index;
  while (current != NULL) {
    printf("%d ; ", current->val);
    current = current->next;
  }
  printf("}\n");
}



// -----------------------------------
// translates a qcblock to a qclist
// -----------------------------------
qclist_t qclist_from_qcblock(qcblock_t h) {
  qclist_t l = qclist_init(h->length);
  int j;
  for (j=0;j<h->weight;++j) {
    qclist_add(l, h->index[j]);
  }
  return l;
}



// -----------------------------------
// translates a qcblock to a qclist
// -----------------------------------
qcblock_t qcblock_from_qclist(qclist_t l) {
  qcblock_t h = qcblock_new(qclist_length(l), qclist_weight(l));
  node_t current = l->index;
  int i = qclist_weight(l)-1;
  while (current != NULL) {
    qcblock_index(h, i) = current->val;
    current = current->next;
    --i;
  }
  return h;
}






// ********************************************************
// ********************************************************

// Distance Spectrum

// ********************************************************
// ********************************************************




// -----------------------------------
// returns the distance spectrum of e
// -----------------------------------
qcsynd_t spectrum(qcblock_t e) { 
  // TODO2 multiplicite ? redef type
  int p = qcblock_length(e);
  int w = qcblock_weight(e);
  qcsynd_t spectre = qcsynd_new(p/2);
  int i, j, d1, d2, d;
  
  for (i=0; i<w-1; ++i) {
    for (j=i+1; j<w; ++j) {
      d1 = qcblock_index(e,j) - qcblock_index(e,i);
      d2 = p - d1;
      d = d1>=d2?d2:d1 ;
      if((spectre)->coeff[d-1]<4) { (spectre)->coeff[d-1]+=1; }
    }
  }
  return spectre;
}



// -----------------------------------
// adds the value v to all the columns that appear in the spectrum of e
// -----------------------------------
void spectrum_add_to_counters(dist_count_t * sweight_counter, 
			     dist_count_t * dist_freq_counter, 
			     qcsynd_t spectrum, 
			     int sw) 
{
  for (int i = 0; i < qcsynd_length(spectrum); ++i) {
    if (qcsynd_coeff(spectrum, i)>=1) {
      dist_freq_counter[i]+=1;
      sweight_counter[i]+=sw;
    }
  }
}




// -----------------------------------
// returns the spectrum of size p-1 containing all indices of the counter 
// with values lower thant the threshold
// -----------------------------------
qcsynd_t spectrum_from_counter(dist_count_t * counter, int p, int threshold) {
  int i;
  qcsynd_t spectre = qcsynd_new(p-1);
  for (i=0; i<p; ++i) {
    if (counter[i]<threshold) {
	qcsynd_set_coeff(spectre, i);
      }
  }
  return spectre;
}



// -----------------------------------
// tests if adding bit i to the list contradicts the spectrum
// -----------------------------------
char spectrum_test_new_bit(qcsynd_t s, qclist_t lk, int i) {
  /* printf("Test de - %d - dans ", i); */
  /* qclist_print(lk, ""); */
  node_t current = lk->index;
  while (current != NULL) {
    if (current->val == i || !(qcsynd_coeff(s, abs(current->val - i)-1 ))) {
      /* printf(" : FAIL with %d\n", current->val); */
      return 0;
    }
    current = current->next;
  }
  /* printf(" : SUCCESS\n"); */
  return 1;
}




// -----------------------------------
/* recursively tries to construct a list (by extending lk) of weight w
such that the spectrum of the list in included in spectre
all bits < b have already been tested */
// -----------------------------------
char block_from_spectrum_aux(qcsynd_t spectre, qclist_t lk, int w, int b) {
  /* if (qclist_weight(lk)<8) { */
  /*   printf("poids %d", qclist_weight(lk)); */
  /*   qclist_print(lk, ""); */
  /* } */
  if (qclist_weight(lk)==w) {
    /* printf("FINI ! ->"); */
    /* qclist_print(lk, "lkfinal"); */
    return 1;
  }
  else {
    /* printf("%d - ",list_weight(lk)); */
    int i;
    for (i=b; i<qcsynd_length(spectre); ++i) {
      if (spectrum_test_new_bit(spectre, lk, i)) {
	  qclist_add(lk, i);
	  if (block_from_spectrum_aux(spectre, lk, w, i+1)) {
	    return 1;
	  }
	  qclist_remove(lk);
      }
    }
    /* printf("\t echec \n"); */
    return 0;
  }
}




// -----------------------------------
// returns a block whose spectrum in included in the input spectrum
// -----------------------------------
qcblock_t block_from_spectrum(qcsynd_t spectrum, int w) {
  qclist_t lk = qclist_init(qcsynd_length(spectrum)+1);
  block_from_spectrum_aux(spectrum, lk, w, 0);
  return qcblock_from_qclist(lk); 
}















// ********************************************************
// ********************************************************

// Test functions

// ********************************************************
// ********************************************************



char is_in_dist(int d, qcblock_t b) {
  qcsynd_t spectre = spectrum(b);
  return qcsynd_coeff(spectre,d);
}

void stat_syndrom_weight(int p, int wh, int we, int d, int N, int se, int sH) { 
  qcmdpc_t H = qcmdpc_new(); 
  qcblock_t e = qcblock_new(0,0) ;
  qcsynd_t s = qcsynd_new(0);
  int ws_cum_0 = 0, ws_cum_1 = 0;
  int N0 = 0, N1 = 0;

  // Creation of the code
  do {
  mysrnd(sH);
  H = qcmdpc_rand(p, wh, myrnd);
  sH++;
  } while (!(is_in_dist(d,qcmdpc_block(H,0))));

  while (N0<N || N1<N) {

    // Creation of the error
    mysrnd(se);
    e = qcblock_rand(p, we, myrnd);
    se++;
    // Computation of the syndrom weight
    s = qcmdpc_synd(H, e);
    
    if (is_in_dist(d, e)) {
      ws_cum_1 += qcsynd_weight(s);
      N1++;
    }
    else {
      ws_cum_0 += qcsynd_weight(s);
      N0++;
    }

  }

  float ratio_0 = ((float) ws_cum_0) / N0;
  float ratio_1 = ((float) ws_cum_1) / N1;
  
  printf("sigma0 = %f \t N0 = %d \n", ratio_0, N0);
  printf("sigma1 = %f \t N1 = %d \n", ratio_1, N1);

  qcmdpc_free(H);
  qcblock_free(e);
  qcsynd_free(s);
}



void get_data_syndrom_weight(int p, int wh, int we, int d, int N, int se, int sH) { 
  qcmdpc_t H = qcmdpc_new(); 
  qcblock_t e = qcblock_new(0,0) ;
  qcsynd_t s = qcsynd_new(0);
  int ws = 0;
  
  // Creation of the code
  do {
  mysrnd(sH);
  H = qcmdpc_rand(p, wh, myrnd);
  sH++;
  } while (!(is_in_dist(d,qcmdpc_block(H,0))));

  dist_count_t * weight_counter = dist_count_new(p);

  for (int i=0; i<N; i++) {

    do {
    // Creation of the error
    mysrnd(se);
    e = qcblock_rand(p, we, myrnd);
    se++;
    }
    while (is_in_dist(d, e)) ;

    // Computation of the syndrom weight
    s = qcmdpc_synd(H, e);
    ws = qcsynd_weight(s);
    weight_counter[ws]+=1;

  }


   FILE *fp;
   fp = fopen("./dat/2.dat", "w+");
   fprintf(fp, "#Nombre d'essais = %d, \n#Longueur du vecteur = %d, \n#Poids de l'erreur = %d, \n#Poids de h = %d. \n", N, p, we, wh);

   for (int i=0; i<p; i++) {
     fprintf(fp, "%d \t %d \n", i, weight_counter[i]);
   }
   fclose(fp);  

  qcmdpc_free(H);
  qcblock_free(e);
  qcsynd_free(s);
  dist_count_free(weight_counter);
}









// -----------------------------------
// computes a counter with the syndroms of N errors
// then reconstructs the block with the threshold (if mentioned)
// or the mean
// -----------------------------------
void test_spectrum_reconstruction(int p, int bl, int bw, int t, int N, int se, int sH) {
  qcblock_t e = qcblock_new(0,0);
  qcsynd_t synd_e = qcsynd_new(0), spectre_e = qcsynd_new(0);
  int sw;
  int ind = 1; // nbblocks


  // Creation of two counters
  dist_count_t * dist_freq_counter = dist_count_new(p/2);
  dist_count_t * sweight_counter = dist_count_new(p/2);
  
  // Creation of the code
  mysrnd(sH);
  qcmdpc_t H = qcmdpc_rand(bl, bw, myrnd);
  
  for (int i=0; i<N; ++i) {

    // Generate a ciphertext
    mysrnd(se);
    e = qcblock_rand(ind * bl, t, myrnd);
    synd_e = qcmdpc_synd(H, e);
    sw = qcsynd_weight(synd_e);
 
    // Compute spectrum
    spectre_e = spectrum(e);

    // Add values to the counters
    spectrum_add_to_counters(sweight_counter, dist_freq_counter, spectre_e, sw);

    // Update seed
    se++;
  }


  dist_count_print(dist_freq_counter, p/2, "freq");
  /* dist_count_print(sweight_counter, p/2, "sweight"); */

  dist_count_float_t * ratio_counter = dist_count_float_new(p/2); // pas des entiers !!!
  for (int i=0; i<p/2; i++) {
    if (dist_freq_counter[i]>0) {
      ratio_counter[i] = sweight_counter[i];
      ratio_counter[i] /= dist_freq_counter[i];
    }
  }

  // print counters
  /* dist_count_float_print(ratio_counter,p/2,"ratio_counter"); */
  /* dist_count_print(sweight_counter,p/2,"sweight"); */

  qcsynd_t h_spectrum = spectrum(qcmdpc_block(H,0));
  write_counts_to(ratio_counter, h_spectrum, p/2, "./dat/1.dat", p, bl, bw, t, N);

  call_gnuplot();

  /* int s=0; */
  /* for (int i=0;i<p/2;i++) { */
  /*   s+=1324-ratio_counter[i]; */
  /* } */
  /* printf("nombre de distances dans le compteur (avec multiplicite) : %d\n", s); */


  dist_count_free(dist_freq_counter);
  dist_count_free(sweight_counter);
  dist_count_float_free(ratio_counter);
  qcmdpc_free(H);
  qcblock_free(e);
  qcsynd_free(synd_e);
  qcsynd_free(spectre_e);
}









// -----------------------------------
// Write counter and synd to dat file
// -----------------------------------
void write_counts_to(dist_count_float_t * ratio_counter, qcsynd_t spectrum_h, int l, char* path, int p, int bl, int bw, int t, int N) {
   FILE *fp;
   fp = fopen(path, "w+");

   fprintf(fp, "#Nombre d'essais = %d, \n#Longueur du vecteur = %d, \n#Poids de l'erreur = %d, \n#Taille d'un bloc = %d, \n#Poids de h = %d. \n", N, p, t, bl, bw);


   for (int i=0; i<l; i++) {
     fprintf(fp, "%d \t %f \t %d \n", i, ratio_counter[i], qcsynd_coeff(spectrum_h,i));
   }
   fclose(fp);  
}




void call_gnuplot() {
  system("gnuplot gnuplot-instructions.gnu > ./dat/1.png");

  /* FILE * f; */
  /* // Ouverture du shell et lancement de gnuplot */
  /* f = popen("gnuplot gnuplot-instructions.gnu > %s" out, "w"); */
  /* /\* // exécution de la commande gnuplot *\/ */
  /* /\* fprintf(f, " plot \"toto.dat\" with lines\n"); *\/ */
  /* /\* fflush(f); *\/ */
  /* // terminer l'envoi de commandes et fermer gnuplot */
  /* sleep(100); */
  /* pclose(f); */
}












// -----------------------------------
// computes the distance spectrum of a random block of given length and weight
// then computes a block whose spectrum is included in this one
// -----------------------------------
void test_block_reconstruction(int length, int weight, int seed) {
  mysrnd(seed);
  qcblock_t h = qcblock_rand(length,weight,myrnd);
  qcblock_print(h, "h0");
  qcsynd_t s = spectrum(h);
  qcblock_t h2 = block_from_spectrum(s, weight);
  qcblock_print(h2, "h2");
  qcsynd_t s2 = spectrum(h2);
  if (qcsynd_inclusion(s2,s)) {
    if (qcsynd_weight(s)==qcsynd_weight(s2)) { 
      /* printf("EQUALITY \t"); */
    }
    else {
      /* printf("INCLUSION \t"); */
    }
  }
  else {
    /* printf("FAIL \t\t"); */
  }
  qcsynd_print(s, "sh");
  qcsynd_print(s2, "sh2");
}




