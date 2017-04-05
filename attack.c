#include "attack.h"






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
  for (i=0; i<vect_length(s)/2; i++) {
    printf("%d ; ", vect_coeff(s, i));
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
    printf("%d - %d\n", vect_coeff(s1, i), vect_coeff(s2, i));
    s+= 1 - (vect_coeff(s1, i) ^ vect_coeff(s2, i));
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
    if(vect_coeff(s1, i)) {
      if(vect_coeff(s2, i)) {
	c11+=1;
      }
      else{
	c10+=1;
      }
    }
    else {
      if(vect_coeff(s2, i)) {
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
  if (!(vect_coeff(s,i))) {
    (s)->coeff[i]=1;
    (s)->weight+=1;
  }
}


// -----------------------------------
// checks if s1 is included in s2
// -----------------------------------
char qcsynd_inclusion(qcsynd_t s1, qcsynd_t s2) {
  if (vect_length(s1)!=vect_length(s2)) {
    return 0;
  }
  for (int i=0; i<vect_length(s1);++i) {
    if (vect_coeff(s1,i)>vect_coeff(s2,i)) {
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
  for (i=0; i<vect_weight(h); ++i) {
    printf("%d ; ", vect_coeff(h, i));
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
// translates a qcblock to a qclist
// -----------------------------------
list_t list_from_qcblock(qcblock_t h) {
  list_t l = list_init(vect_length(h));
  int j;
  for (j=0;j<h->weight;++j) {
    list_add(l, h->coeff[j]);
  }
  return l;
}



// -----------------------------------
// translates a qclist to a qcblock
// -----------------------------------
qcblock_t qcblock_from_list(list_t l) {
  qcblock_t h = qcblock_new(list_p(l), list_length(l));
  node_t current = l->index;
  int i = list_length(l)-1;
  while (current != NULL) {
    vect_coeff(h, i) = current->val;
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
  int p = vect_length(e);
  int w = vect_weight(e);
  qcsynd_t spectre = qcsynd_new(p);
  int i, j, d1, d2, d;
  
  for (i=0; i<w-1; ++i) {
    for (j=i+1; j<w; ++j) {
      d1 = vect_coeff(e,j) - vect_coeff(e,i);
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
  for (int i = 0; i < vect_length(spectrum); ++i) {
    if (vect_coeff(spectrum, i)>=1) {
      dist_freq_counter[i]+=1;
      sweight_counter[i]+=sw;
    }
  }
}




// -----------------------------------
// returns the spectrum of size p-1 containing all indices of the counter 
// with values lower thant the threshold
// -----------------------------------
qcsynd_t spectrum_from_counter(dist_count_float_t * counter, int p, float threshold) {
  qcsynd_t spectre = qcsynd_new(p-1);
  for (int i=0; i<p; ++i) {
    if (counter[i]<threshold) {
	qcsynd_set_coeff(spectre, i);
      }
  }
  return spectre;
}


// -----------------------------------
// returns the distance between i and j with symetry of size p
// -----------------------------------
int spectrum_dist( int i, int j, int p) {
  if (j>i) {
    return MIN(j-i,p-(j-i));
  }
  else {
    return MIN(i-j,p-(i-j));
  }
}
























