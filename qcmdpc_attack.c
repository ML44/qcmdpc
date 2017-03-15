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

/* Renvoie 0 si i dans k ou si il existe j dans k tel que la distance (j-i) n'est pas dans s */
char dist_test(qcsynd_t s, qcblock_t k, int i) {
  int j;
  for (j=0; j<qcblock_weight(k); ++j) {
    /* TODO TEST i dans k */
    if (!(qcsynd_coeff(s,i - qcblock_index(k,j)))){ /* ABS VALUE */
	return 0;
      }
  }
  return 1;
}

/* TODO ajouter le test dans dist_test sinon va fail */
char dist_reconstruct_aux(qcsynd_t spectre, qcblock_t k, int w) {
  if (qcblock_weight(k)==w) {
    return 1;
  }
  else {
    int i;
    for (i=0; i<qcsynd_length(spectre); ++i) {
      if (dist_test(spectre, k, i)) {
	  qcblock_add(k, i);
	  if (dist_reconstruct_aux(spectre, k, w)) {
	    return 1;
	  }
	  qcblock_remove(k, i);
      }
    }
    return 0;
  }
}

/* Pas du tout efficace ... faire des linked lists */
void qcblock_add(qcblock_t k, int i) {
  int j=0;
  int w = k->weight;
  index_t * indexbis = (index_t *) malloc((w+1) * sizeof (index_t));
  while (k->index[j]<i) {
    indexbis[j]=k->index[j];
    ++j;
  }
  indexbis[j]=i;
  ++j;
  while (j<w) {
    indexbis[j+1]=k->index[j];
    ++j;
  }
  k->index = indexbis;
  k->weight++;
}
  

void qcblock_remove(qcblock_t k, int i) {
  int j=0;
  while (qcblock_index(k, j)!=i && j<qcblock_weight(k)){
    ++j;
  }
  for (;j+1<qcblock_weight(k);++j) {
    k->index[j] = k->index[j+1];
  }
  k->weight--;
}

list_t list_init() {
  list_t l = malloc( sizeof ( struct list ) );
  l->index = NULL;
  l->weight=0;
  return l;
}

char list_isempty(list_t l) {
  if (l->index == NULL) {return 1;} return 0;
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
    printf("%d ;", current->val);
    current = current->next;
  }
  printf(" }\n");
}

list_t list_from_qcblock(qcblock_t h) {
  list_t l = list_init();
  int j;
  for (j=0;j<h->weight;++j) {
    list_add(l, h->index[j]);
  }
  return l;
}
