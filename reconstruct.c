#include "reconstruct.h"

char adjacence(int i, int j, qcsynd_t spectrum) { // à utiliser plutot que directement le spectre
  return (i==j) | (vect_coeff(spectrum,spectrum_dist(i,j,vect_length(spectrum))-1)>0);
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
  while (adjacence(0,p1,spectrum)==0) {
    p1+=1;
  }
  return p1;
}

list_t construct_A(qcsynd_t spectrum, int p1) {
  list_t A = list_init(0);
  int p = vect_length(spectrum);
  
  for (int i=p1+1; i<=p; i++) {
    
    if (adjacence(0,i,spectrum) & adjacence(i,p1,spectrum))
      {
	list_add(A,i);
      }
  }
  return A;
}

list_t construct_B(qcsynd_t spectrum, list_t A, int p2) {
  list_t B = list_init(0);
  node_t current = A->index;
  int i;
  
  int k=0;
  do
    {
      i = current->val;
      
      if ((i!=p2) & adjacence(i,p2,spectrum)) {
	list_add_sorted(B,i); 
      }
      
      current = current->next;
      k++;      
    }
  while (k<list_length(A));
  
  return B;
}


list_list_t get_cliques(list_t C, qcsynd_t spectrum, int size) {
  
  list_list_t cliques;
  
  if (list_length(C)==size) {
    cliques = list_list_init();
    list_list_add(cliques,C);
  }
  
  else 
    {
      qcblock_t vertices = qcblock_new(list_p(C),list_length(C));
      node_t current = C->index;
      for(int i=0; i<list_length(C); i++) 
	{
	  vect_coeff(vertices, i) = current->val;
	  current = current->next;
	}
      
      
      graph_t g = graph_new(vertices, spectrum);
      cliques = graph_findMaxClique(g);
      
      /* list_print(C,"C"); */
      /* qcblock_print(vertices, "v");       */
      /* list_list_print(cliques,"cliques"); */
      
    }
  
  

  return cliques;
}



list_list_t dsr(qcsynd_t spectrum, int weight) {
  int p1 = get_p1(spectrum);
  list_t A = construct_A(spectrum, p1);
  list_list_t E = list_list_init();

  node_t currentA = A->index;
  int p2;
  
  int kA=0;
  do
    {
      p2 = currentA->val;
      // IN THE A LOOP



      list_t B = construct_B(spectrum, A, p2);
      list_add_sorted(B,p2); // add sorted ?
      list_add(B,p1);
      list_add(B,0);


      char b = 1;
      while ((list_length(B)>=weight) & b) {
	
	/* printf("w = %d : ",list_length(B));	 */
	/* list_print(B,"B = "); */
	

	node_t currentB = B->index;
	int iB;

	int kB=0;
	do 
	  {
	    iB = currentB->val;
	    // IN THE B LOOP
	    
	    int c = 0;
	      

	    node_t currentB2 = B->index;
	    int jB;
	    
	    int kB2=0;
	    do 
	      {
		jB = currentB2->val;
		// IN THE B2 LOOP


	    	if (adjacence(iB, jB, spectrum)) {
		  /* printf("Checking (%d, %d) : ", iB, jB);		 */
	    	  /* /\* printf("PASSED\n"); *\/ */
		  
		  c++;
	    	}
		/* else  */
		/*   { */
		/*     printf("Checking (%d, %d) : ", iB, jB);		 */
		/*     printf("FAILED\n");   */
		/*   } */
		

		// END OF B2 LOOP
		currentB2 = currentB2->next;
		kB2++;      
	      }
	    while (kB2<list_length(B));



	    if (c<weight) {
	      /* printf("ENLEVER ELEMENT %d \n", kB); */
	      list_remove_ith(B,kB);
	      break;
	    }


	    if (kB == list_length(B) - 1) {
	      b = 0;
	    }


	    // END OF B LOOP
	    currentB = currentB->next;
	    kB++;      
	  }
	while (kB<list_length(B));
      
      }
      
      if (!(b)) {
	list_list_add(E,B);
      }

      // END OF A LOOP
      currentA = currentA->next;
      kA++;      
    }
  while (kA<list_length(A));



  /* list_list_print(E,"E"); */
  



  list_list_t F = list_list_init();
  
  list_node_t currentE = E->index;
  list_t S;
  int kE=0;
  do 
    {
      S = currentE->val;
      // IN THE E LOOP

      list_list_t K = get_cliques(S, spectrum, weight); // TODO 
      /* Pour le moment renvoit K = [S] */

      /* list_list_print(K,"K"); */
      /* list_list_print(K,"F"); */


      list_node_t currentK = K->index;
      list_t iK;
      int kK=0;
      
      do 
	{
	  iK = currentK->val;
	  // IN THE K LOOP
	  
	  if(!(list_list_is_in(F, iK))) 
	    {
	      list_list_add(F,iK); // sans doublons
	    }
	  
	  // END OF K LOOP
	  currentK = currentK->next;
	  kK++;      
	}
      while (kK<list_length(K));
      
      // END OF B2 LOOP
      currentE = currentE->next;
      kE++;      
    }
  while (kE<list_length(E));

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
