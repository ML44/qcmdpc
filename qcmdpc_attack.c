#include "qcmdpc_attack.h"

// ********************************************************
// ********************************************************

// Random 

// ********************************************************
// ********************************************************


// -----------------------------------
// -----------------------------------
int myrnd() { return random(); }


// -----------------------------------
// -----------------------------------
void mysrnd(int seed) { srandom(seed); }





// ********************************************************
// ********************************************************

// Qcsynd

// ********************************************************
// ********************************************************



// -----------------------------------
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
// -----------------------------------
void qcsynd_set_coeff(qcsynd_t s, int i) {
  if (!(qcsynd_coeff(s,i))) {
    (s)->coeff[i]=1;
    (s)->weight+=1;
  }
}


// -----------------------------------
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
// alloue un compteur de taille donnee
// -----------------------------------
dist_count_t * dist_count_new(int length) {
	dist_count_t * counter;
	counter = (dist_count_t *) calloc(length, sizeof (dist_count_t));
	return counter;
}




// -----------------------------------
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
// -----------------------------------
void dist_count_free(dist_count_t * counter) {
	free(counter);
}




// -----------------------------------
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

// Qclist

// ********************************************************
// ********************************************************



// -----------------------------------
// -----------------------------------
qclist_t qclist_init(int len) {
  qclist_t l = malloc( sizeof ( struct qclist ) );
  l->index = NULL;
  l->weight=0;
  l->length=len;
  return l;
}



// -----------------------------------
// -----------------------------------
char qclist_isempty(qclist_t l) {
  if (l->index == NULL) {return 1;} return 0;
}



// -----------------------------------
// -----------------------------------
void qclist_add(qclist_t l, index_t v) {
  node_t head = malloc(sizeof(node_t));
  head->val = v;
  head->next = l->index;
  l->index = head;
  l->weight++;
}



// -----------------------------------
// -----------------------------------
void qclist_remove(qclist_t l) {
  if(l->weight>0) {
    l->index = l->index->next;
    l->weight--;
  }
}



// -----------------------------------
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
// renvoie le spectre de distances de e
// -----------------------------------
qcsynd_t spectrum(qcblock_t e) {
  int p = qcblock_length(e);
  int w = qcblock_weight(e);
  qcsynd_t spectre = qcsynd_new(p-1);
  int i, j, d1, d2;
  
  for (i=0; i<w-1; ++i) {
    for (j=i+1; j<w; ++j) {
      d1 = qcblock_index(e,j) - qcblock_index(e,i);
      d2 = qcblock_length(e) - d1;
      /* d = d1>=d2?d2:d1 ; */
      qcsynd_set_coeff(spectre, d1-1);
      qcsynd_set_coeff(spectre, d2-1);
    }
  }
  return spectre;
}



// -----------------------------------
// ajoute la valeur ws a toutes les valeurs presentes dans le spectre de e
// -----------------------------------
void spectrum_add_to_counter(dist_count_t * counter, qcsynd_t spectre, int ws) {
  int i;
  
  for (i = 0; i < qcsynd_length(spectre); ++i) {
    if (qcsynd_coeff(spectre, i)) {
      counter[i]+=ws;
    }
  }
}




// -----------------------------------
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
// -----------------------------------
qcblock_t block_from_spectrum(qcsynd_t spectre, int w) {
  qclist_t lk = qclist_init(qcsynd_length(spectre)+1);
  /* qclist_add(lk, 0); */
  block_from_spectrum_aux(spectre, lk, w, 0);
  return qcblock_from_qclist(lk); 
}















// ********************************************************
// ********************************************************

// Test functions

// ********************************************************
// ********************************************************



// -----------------------------------
// -----------------------------------
void test_spectrum_reconstruction(int p, int bl, int bw, int t, int N, int seuil, int se, int sH) {
  qcblock_t h, e = qcblock_new(0,0);
  qcsynd_t synd_e = qcsynd_new(0), spectre_e = qcsynd_new(0), 
    spectre_h, spectre_h_reconstruct;
  dist_count_t * compteur;
  qcmdpc_t H;
  int ws, m;
  int ind = 1; // TODO adapter à plusieurs blocs

  printf("Nombre d'essais = %d, \nLongueur du vecteur = %d, \nPoids de l'erreur = %d, \nNombre de blocs = %d, \nTaille d'un bloc = %d, \nPoids de h = %d. \n", N, ind*p, t, ind, bl, bw);
  
  compteur = dist_count_new(p-1);
  mysrnd(sH);
  H = qcmdpc_rand(bl, bw, myrnd);
  
  while (--N) {
    //printf("\n");
    mysrnd(se);
    e = qcblock_rand(ind * bl, t, myrnd);
    synd_e = qcmdpc_synd(H, e);
    //qcblock_print(e, "e");
    //qcsynd_print(synd_e, "s");
    ws = qcsynd_weight(synd_e);
    //printf("Poids de s : %d\n", ws);
    spectre_e = spectrum(e);
    //qcsynd_print(spectre_e, "spectre_e");
    spectrum_add_to_counter(compteur, spectre_e, ws);
    //dist_count_print(compteur, p-1, "compteur cumule");
    se++;
  }

  //printf("\n ======== \n");

  //dist_count_print(compteur, p-1, "compteur cumule");
  m = dist_count_mean(compteur, p-1);
  seuil = (seuil == 0) ? m : seuil;
  spectre_h_reconstruct = spectrum_from_counter(compteur, p-1, seuil);
  //qcsynd_print(spectre_h_reconstruct, "spectre_h deduit");
  
  h = qcmdpc_block(H,0);
  spectre_h = spectrum(h);
  // TODO spectre cumule par bloc de h
  // spectre_h_cum = qcsynd_new(h->length, 0).
  //qcblock_print(h, "h");
  //qcsynd_print(spectre_h, "spectre_h reel  ");

  printf("moyenne = %d\n", m);
  printf("seuil = %d\n", seuil);

  //qcsynd_print2(spectre_h, spectre_h_reconstruct, p-1);
  qcsynd_compare(spectre_h, spectre_h_reconstruct, p-1);
  
  // free
  qcblock_free(e);
  qcblock_free(h);
  qcsynd_free(synd_e);
  qcsynd_free(spectre_e);
  qcsynd_free(spectre_h);
  dist_count_free(compteur);
}





// -----------------------------------
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
