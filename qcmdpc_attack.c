#include "qcmdpc_attack.h"

// alloue un compteur de taille donnee
dist_count_t * dist_count_new(int length) {
	dist_count_t * counter;
	counter = (dist_count_t *) calloc(length, sizeof (dist_count_t));
	return counter;
}

// renvoie le spectre de distances de e
qcsynd_t dist_spectre(qcblock_t e) {
  int p = qcblock_length(e);
  int w = qcblock_weight(e);
  qcsynd_t spectre = qcsynd_new(p-1);
  int i, j, d1, d2, d;
  
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


// TODO inplace add spectre
/* void dist_spectre_or(qcsynd_t e) { */
/*   int p = qcblock_length(e); */
/*   int w = qcblock_weight(e); */
/*   qcsynd_t spectre = qcsynd_new(p-1); */
/*   int i, j, d1, d2, d; */
  
/*   for (i=0; i<w-1; ++i) { */
/*     for (j=i+1; j<w; ++j) { */
/*       d1 = qcblock_index(e,j) - qcblock_index(e,i); */
/*       d2 = qcblock_length(e) - d1; */
/*       /\* d = d1>=d2?d2:d1 ; *\/ */
/*       qcsynd_set_coeff(spectre, d1-1); */
/*       qcsynd_set_coeff(spectre, d2-1); */
/*     } */
/*   } */
/*   return spectre; */
/* } */


// ajoute la valeur ws a toutes les valeurs presentes dans le spectre de e
void dist_count_add_spectre(dist_count_t * counter, qcsynd_t spectre, int ws) {
  int i;
  
  for (i = 0; i < qcsynd_length(spectre); ++i) {
    if (qcsynd_coeff(spectre, i)) {
      counter[i]+=ws;
    }
  }
}

void dist_count_print(dist_count_t * counter, int p, char * str) {
  int i;
  printf("%s = { ", str);
  for (i=0; i<p; i++) {
    printf("%d ; ", counter[i]);
  }
  printf("} \n");
}

void dist_count_free(dist_count_t * counter) {
	free(counter);
}

void qcsynd_print(qcsynd_t s, char * str) {
  int i;
  printf("%s = { ", str);
  for (i=0; i<qcsynd_length(s); i++) {
    printf("%d ; ", qcsynd_coeff(s, i));
  }
  printf("} \n");
}

void qcsynd_print2(qcsynd_t s1, qcsynd_t s2, int l) {
  int i;
  int s = 0;
  for (i=0; i<l; i++) {
    printf("%d - %d\n", qcsynd_coeff(s1, i), qcsynd_coeff(s2, i));
    s+= 1 - (qcsynd_coeff(s1, i) ^ qcsynd_coeff(s2, i));
  }
  printf("similarite : %d / %d\n", s, l);
}

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


void qcblock_print(qcblock_t h, char * str) {
  int i;
  printf("%s = ", str);
  for (i=0; i<qcblock_weight(h); i++) {
    printf("%d ; ", qcblock_index(h, i));
  }
  printf("\n");
}

int dist_count_mean(dist_count_t * counter, int p) {
  unsigned long int m = 0;
  int i;
  for (i=0; i<p; i++) {
    m += counter[i];
  }
  return m/p;
}

qcsynd_t dist_spectre_reconstruct(dist_count_t * counter, int p, int m) {
  int i;
  qcsynd_t spectre = qcsynd_new(p-1);
  for (i=0; i<p; i++) {
    if (counter[i]<m) {
	qcsynd_set_coeff(spectre, i);
      }
  }
  return spectre;
}
