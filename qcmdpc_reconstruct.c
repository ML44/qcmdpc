#include "qcmdpc_reconstruct.h"





char adjacence(int i, int j, qcsynd_t spectrum) { // à utiliser plutot que directement le spectre
  return spectrum->coeff[MIN(abs(j-i),abs(i-j))];
}




qclist_t get_neighbours(int v, qcblock_t S, qcsynd_t spectrum) {
  qclist_t n = qclist_init(0);
  for (int i=0; i<qcblock_weight(S); i++) {
    int j = qcblock_index(S,i);
    if (j!=v && adjacence(v,j,spectrum)) {
      qclist_add(n,qcblock_index(S,i));
    }
  }
  return n;
}

int get_p1(qcsynd_t spectrum){
  int p1 = 1;
  while (qcsynd_coeff(spectrum,p1)==0){
    p1+=1;
  }
  return p1;
}

qclist_t construct_A(qcsynd_t spectrum, int p1) {
  qclist_t A = qclist_init(0);
  for (int i=p1+1; i<qcsynd_length(spectrum); i++) {
    if (qcsynd_coeff(spectrum,i) && qcsynd_coeff(spectrum,(i-p1))) {
      qclist_add(A,i);
    }
  }
  return A;
}

qclist_t construct_B(qcsynd_t spectrum, qclist_t A, int p2) {
  qclist_t B = qclist_init(0);
  node_t current = A->index;
  int i = current->val;
  
  for (int k=0; k<qclist_weight(A); k++) {	
    if (i!=p2 && qcsynd_coeff(spectrum,i-p2)) {
	  qclist_add(B,i);
	}
    current = current->next;
    i = current->val;
  }
  return B;
}


qclist_list_t get_cliques(qclist_t C, qcsynd_t spectrum, int weight) {
  qclist_list_t l = qclist_list_init(0);
  if (qclist_weight(C)==weight) {
    /* qclist_sort(C); */
    qclist_list_add(l,C);
  }
  else {
    qclist_t e1 = qclist_init(0);
    qclist_t e2 = qclist_init(0);

    /* TODO */
    /* qclist_list_t cliques = graph_bkv2(C, spectrum, e1, C, e2, l, weight); */
    
    /* qclist_node_t current_clique = cliques->index; */
    /* qclist_t c = current_clique->val; */
    
    /* for (int k=0; k<cliques->weight; k++) { */
    /*   /\* qclist_sort(c); *\/ */
    /*   /\* if (!(c in l)) { // NON *\/ */
    /*   qclist_list_add(l,c); */
    /*   /\* } *\/ */
    /* } */
  }
  return l;
}



qclist_list_t dsr(qcsynd_t spectrum, int weight) {
  int p1 = get_p1(spectrum);
  qclist_t A = construct_A(spectrum, p1);
  qclist_list_t E = qclist_list_init(0);

  node_t currentA = A->index;
  int p2 = currentA->val;
  
  for (int k=0; k<A->weight; k++) { 
    qclist_t B = construct_B(spectrum, A, p2);
    C = [0,p1,p2];
    C.extend(B);
    C.sort() ;
    b = True;
    while (len(C)>=weight and b) {
      for (i in C) {
	c = 0;
                for j in C:
                    if spectrum[abs(j-i)]:
                        c+=1
                if c<weight:
                    C.remove(i)
                    break
                if i == C[-1]:
                    b = False
        if not(b):
            E.append(C)
	      }
    }
    currentA = currentA->next;
    p2 = currentA->val;
  }

  qclist_list_t F = qclist_list_new(0);
  for (S in E) {
    qclist_t K = get_cliques(S, spectrum, weight);
    for (k in K) {
      if not(k in F) {
	  F.append(k);
	}
    }
  }
  return F;
}
