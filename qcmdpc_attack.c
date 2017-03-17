#include "qcmdpc_attack.h"


// gestion de l'alea 
long random(void);
void srandom(unsigned int seed);
int myrnd() { return random(); }
void mysrnd(int seed) { srandom(seed); }


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
  for (i=0; i<qcblock_weight(h); ++i) {
    printf("%d ; ", qcblock_index(h, i));
  }
  printf("\n");
}

int dist_count_mean(dist_count_t * counter, int p) {
  unsigned long int m = 0;
  int i;
  for (i=0; i<p; ++i) {
    m += counter[i];
  }
  return m/p;
}

qcsynd_t dist_spectre_reconstruct(dist_count_t * counter, int p, int m) {
  int i;
  qcsynd_t spectre = qcsynd_new(p-1);
  for (i=0; i<p; ++i) {
    if (counter[i]<m) {
	qcsynd_set_coeff(spectre, i);
      }
  }
  return spectre;
}

char dist_test(qcsynd_t s, list_t lk, int i) {
  /* printf("Test de - %d - dans ", i); */
  /* list_print(lk, ""); */
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

/* recursively tries to construct a list (by extending lk) of weight w
such that the spectrum of the list in included in spectre
all bits < b have already been tested */
char dist_reconstruct_aux(qcsynd_t spectre, list_t lk, int w, int b) {
  /* if (list_weight(lk)<8) { */
  /*   printf("poids %d", list_weight(lk)); */
  /*   list_print(lk, ""); */
  /* } */
  if (list_weight(lk)==w) {
    /* printf("FINI ! ->"); */
    /* list_print(lk, "lkfinal"); */
    return 1;
  }
  else {
    /* printf("%d - ",list_weight(lk)); */
    int i;
    for (i=b; i<qcsynd_length(spectre); ++i) {
      if (dist_test(spectre, lk, i)) {
	  list_add(lk, i);
	  if (dist_reconstruct_aux(spectre, lk, w, i+1)) {
	    return 1;
	  }
	  list_remove(lk);
      }
    }
    /* printf("\t echec \n"); */
    return 0;
  }
}

qcblock_t dist_reconstruct(qcsynd_t spectre, int w) {
  list_t lk = list_init(qcsynd_length(spectre)+1);
  dist_reconstruct_aux(spectre, lk, w, 0);
  return qcblock_from_list(lk); 
}

list_t list_init(int len) {
  list_t l = malloc( sizeof ( struct list ) );
  l->index = NULL;
  l->weight=0;
  l->length=len;
  return l;
}

char list_isempty(list_t l) {
  if (l->index == NULL) {return 1;} return 0;
}

void qcsynd_set_coeff(qcsynd_t s, int i) {
  if (!(qcsynd_coeff(s,i))) {
    (s)->coeff[i]=1;
    (s)->weight+=1;
  }
}

void list_add(list_t l, index_t v) {
  node_t head = malloc(sizeof(node_t));
  head->val = v;
  head->next = l->index;
  l->index = head;
  l->weight++;
}

void list_remove(list_t l) {
  if(l->weight>0) {
    l->index = l->index->next;
    l->weight--;
  }
}

void list_print(list_t l, char * str) {
  printf("%s = { ", str);
  node_t current = l->index;
  while (current != NULL) {
    printf("%d ; ", current->val);
    current = current->next;
  }
  printf("}\n");
}

list_t list_from_qcblock(qcblock_t h) {
  list_t l = list_init(h->length);
  int j;
  for (j=0;j<h->weight;++j) {
    list_add(l, h->index[j]);
  }
  return l;
}

qcblock_t qcblock_from_list(list_t l) {
  qcblock_t h = qcblock_new(list_length(l), list_weight(l));
  node_t current = l->index;
  int i = list_weight(l)-1;
  while (current != NULL) {
    qcblock_index(h, i) = current->val;
    current = current->next;
    --i;
  }
  return h;
}

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

void test_reconstruct(int length, int weight, int seed) {
  mysrnd(seed);
  qcblock_t h = qcblock_rand(length,weight,myrnd);
  /* qcblock_print(h, "h0"); */
  qcsynd_t s = dist_spectre(h);
  qcblock_t h2 = dist_reconstruct(s, weight);
  /* qcblock_print(h2, "h2"); */
  qcsynd_t s2 = dist_spectre(h2);
  if (qcsynd_inclusion(s2,s)) {
    if (qcsynd_weight(s)==qcsynd_weight(s2)) { 
      printf("EQUALITY \t");
    }
    else {
      printf("INCLUSION \t");
    }
  }
  else {
    printf("FAIL \t\t");
  }
  /* qcsynd_print(s, "sh"); */
  /* qcsynd_print(s2, "sh2"); */
}
