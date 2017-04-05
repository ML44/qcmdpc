#include "reconstruct.h"

char adjacence(int i, int j, qcsynd_t spectrum) { // à utiliser plutot que directement le spectre
  return vect_coeff(spectrum,spectrum_dist(i,j,vect_length(spectrum))-1);
}

list_t get_neighbours(int v, qcblock_t S, qcsynd_t spectrum) {
  list_t n = list_init(vect_length(S));
  for (int i=0; i<vect_weight(S); i++) {
    int j = vect_coeff(S,i); 
    if (v!=j && adjacence(v,j,spectrum)) {
      list_add(n,vect_coeff(S,i));
    }
  }
  return n;
}

int get_p1(qcsynd_t spectrum){
  int p1 = 1;
  while (vect_coeff(spectrum,p1-1)==0){
    p1+=1;
  }
  return p1;
}

list_t construct_A(qcsynd_t spectrum, int p1) {
  list_t A = list_init(0);
  for (int i=p1+1; i<vect_length(spectrum); i++) {
    if (vect_coeff(spectrum,i) && vect_coeff(spectrum,(i-p1))) {
      list_add(A,i);
    }
  }
  return A;
}

list_t construct_B(qcsynd_t spectrum, list_t A, int p2) {
  list_t B = list_init(0);
  node_t current = A->index;
  int i = current->val;
  
  for (int k=0; k<list_length(A); k++) {	
    if (i!=p2 && vect_coeff(spectrum,i-p2)) {
	  list_add(B,i);
	}
    current = current->next;
    i = current->val;
  }
  return B;
}


list_list_t get_cliques(list_t C, qcsynd_t spectrum, int size) {
  list_list_t l = list_list_init();
  if (list_length(C)==size) {
    list_sort(C); // does nothing
    list_list_add(l,C);
  }
  else {
    /* TODO */

    /* list_t e1 = list_init(0); */
    /* list_t e2 = list_init(0); */

    /* list_list_t cliques = graph_bkv2(C, spectrum, e1, C, e2, l, weight); */
    
    /* list_node_t current_clique = cliques->index; */
    /* list_t c = current_clique->val; */
    
    /* for (int k=0; k<cliques->weight; k++) { */
    /*   /\* list_sort(c); *\/ */
    /*   /\* if (!(c in l)) { // NON *\/ */
    /*   list_list_add(l,c); */
    /*   /\* } *\/ */
    /* } */
  }
  return l;
}



list_list_t dsr(qcsynd_t spectrum, int weight) {
  int p1 = get_p1(spectrum);
  list_t A = construct_A(spectrum, p1);
  list_list_t E = list_list_init();

  node_t currentA = A->index;
  int p2 = currentA->val;
  
  for (int kA=0; kA<A->length; kA++) { 
    list_t B = construct_B(spectrum, A, p2);
    list_t C = list_init(vect_length(spectrum));
    list_add(C,0);
    list_add(C,p1);
    list_add(C,p2);
    list_extend(C,B);
    list_sort(C) ; // does nothing
    char b = 1;
    while ((list_length(C)>=weight) & b) {
      //TODO
      node_t currentC = C->index;
      int i = currentC->val;

      for (int kC=0; kC<C->length; kC++) {
	int c = 0;

	node_t currentC2 = C->index;
	int j = currentC2->val;

	for (int kC2=0; kC2<C->length; kC2++) {
	  if (adjacence(i, j, spectrum)) {
	    c++;
	  }

	currentC2 = currentC2->next;
	j = currentC2->val;
	}
	
	
	if (c<weight) {
	  list_remove_ith(C,i); // TODO verifier que marche
	  break;
	}
	
	if (kC == C->length - 1) { // ???
	  b = 0; 
	}

	currentC = currentC->next;
	i = currentC->val;
      }
    }
    
    if (!(b)) {
	list_list_add(E,C);
      }
    
    currentA = currentA->next;
    p2 = currentA->val;
  }

  list_list_t F = list_list_init();
  list_node_t currentE = E->index;
  list_t S = currentE->val;
  for (int kS=0; kS<E->length; kS++) {
    list_list_t K = get_cliques(S, spectrum, weight);
    /* for (k in K) { */
    /*   if not(k in F) { */
    /* list_list_add(F,k); */
	/* } */
    /* } */
    list_list_extend(F,K);
    
    currentE = currentE->next;
    S = currentE->val;
  }
  return F;
}





// ********************************************************
// ********************************************************

// Test functions

// ********************************************************
// ********************************************************












// -----------------------------------
// tests if adding bit i to the list contradicts the spectrum
// -----------------------------------
char spectrum_test_new_bit(qcsynd_t s, list_t lk, int i) {
  /* printf("Test de - %d - dans ", i); */
  /* qclist_print(lk, ""); */
  node_t current = lk->index;
  while (current != NULL) {
    if (current->val == i || !(vect_coeff(s, abs(current->val - i)-1 ))) {
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
char block_from_spectrum_aux(qcsynd_t spectre, list_t lk, int w, int b) {
  /* if (qclist_weight(lk)<8) { */
  /*   printf("poids %d", qclist_weight(lk)); */
  /*   qclist_print(lk, ""); */
  /* } */
  if (list_length(lk)==w) {
    /* printf("FINI ! ->"); */
    /* qclist_print(lk, "lkfinal"); */
    return 1;
  }
  else {
    /* printf("%d - ",list_weight(lk)); */
    int i;
    for (i=b; i<vect_length(spectre); ++i) {
      if (spectrum_test_new_bit(spectre, lk, i)) {
	  list_add(lk, i);
	  if (block_from_spectrum_aux(spectre, lk, w, i+1)) {
	    return 1;
	  }
	  list_remove(lk);
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
  list_t lk = list_init(2*vect_length(spectrum)+1);
  block_from_spectrum_aux(spectrum, lk, w, 0);
  return qcblock_from_list(lk); 
}




// -----------------------------------
// computes the distance spectrum of a random block of given length and weight
// then computes a block whose spectrum is included in this one
// -----------------------------------
void block_reconstruction(int p, int w, int sh) {

  // feed random generator
  mysrnd(sh);
  // generate h
  qcblock_t h0 = qcblock_rand(p,w,myrnd);
  qcblock_print(h0, "h0");
  // compute its spectrum
  qcsynd_t s0 = spectrum(h0);
  qcsynd_print(s0, "s0");

  // reconstruct a polynomial matching the spectrum
  qcblock_t h1 = block_from_spectrum(s0, w);
  qcblock_print(h1, "h1");
  // compute its spectrum
  qcsynd_t s1 = spectrum(h1);
  qcsynd_print(s1, "s1");
  
  // test inclusion, equality
  if (qcsynd_inclusion(s1,s0)) {
    if (vect_weight(s0)==vect_weight(s1)) { 
      printf("EQUALITY \t");
    }
    else {
      printf("INCLUSION \t");
    }
  }
  else {
    printf("FAIL \t\t");
  }
  printf("\n");
}
