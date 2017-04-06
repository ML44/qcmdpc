# include "lib.h"





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

// Lists

// ********************************************************
// ********************************************************



// -----------------------------------
// new empty list
// -----------------------------------
list_t list_init( int p ) {
  list_t l = malloc( sizeof ( struct list ) );
  l->index = NULL;
  l->p = p;
  l->length=0;
  return l;
}



// -----------------------------------
// tests if list is empty
// -----------------------------------
char list_isempty(list_t l) {
  if (l->length==0) {return 1;} return 0;
}



// -----------------------------------
// adds value v at the beginning of the list
// -----------------------------------
void list_add(list_t l, index_t v) {
  node_t head = malloc(sizeof(node_t));
  head->val = v;
  head->next = l->index;
  l->index = head;
  l->length++;
}



// -----------------------------------
// removes the first value of the list
// -----------------------------------
void list_remove(list_t l) {
  if(l->length>0) {
    l->index = l->index->next;
    l->length--;
  }
}


// -----------------------------------
// adds list l2 at the end of the list l1
// -----------------------------------
void list_extend(list_t l1, list_t l2) {
  node_t current = l1->index;
  for (int i=0; i<l1->length; i++) {
    current = current->next;
  }
  current->next = l2->index;
  l1->length += l2->length;
}


// -----------------------------------
// sorts the list
// -----------------------------------
void list_sort(list_t l) {
  
}
  




// -----------------------------------
// prints the list with str prefix message
// -----------------------------------
void list_print(list_t l, char * str) {
  printf("%s = { ", str);
  node_t current = l->index;
  for (int i=0; i<l->length; i++) {
    printf("%d ; ", current->val);
    current = current->next;
  }
  printf("}\n");
}





// -----------------------------------
// removes the first value of the list
// -----------------------------------
void list_remove_ith(list_t l, int i) {
  if (i==0) 
    {
      l->index = l->index->next;
      l->length-=1;
    }
  else
    {
      node_t current = l->index;
      int k=0;
      
      while (k<i-1)
	{
	  current = current->next;
	  k++;
	};
      
      current->next = (current->next)->next;
      l->length-=1;
      
    }
  }



















// ********************************************************
// ********************************************************

// Lists

// ********************************************************
// ********************************************************





// -----------------------------------
// new empty lists of list
// -----------------------------------
list_list_t list_list_init( void ) {
  list_list_t l = malloc( sizeof ( struct list_list ) );
  l->index = NULL;
  l->length=0;
  return l;
}



// -----------------------------------
// tests if list is empty
// -----------------------------------
char list_list_isempty(list_list_t l) {
  if (l->length == 0) {return 1;} return 0;
}



// -----------------------------------
// adds value v at the beginning of the list
// -----------------------------------
void list_list_add(list_list_t l, list_t v) {
  list_node_t head = malloc(sizeof(list_node_t));
  head->val = v;
  head->next = l->index;
  l->index = head;
  l->length++;
}



// -----------------------------------
// removes the first value of the list
// -----------------------------------
void list_list_remove(list_list_t l) {
  if(l->length>0) {
    l->index = l->index->next;
    l->length--;
  }
}



// -----------------------------------
// prints the list with str prefix message
// -----------------------------------
void list_list_print(list_list_t l, char * str) {
  printf("%s = { \n", str);
  list_node_t current = l->index;
  for (int i=0; i<l->length; i++) 
    {
      list_print(current->val, "");
      current = current->next;
    }
  printf("}\n");
}


// -----------------------------------
// adds list l2 at the end of the list l1
// -----------------------------------
void list_list_extend(list_list_t l1, list_list_t l2) {
  list_node_t current = l1->index;
  for (int i=0; i<l1->length; i++) {
    current = current->next;
  }
  current->next = l2->index;
  l1->length += l2->length;
}







// ********************************************************
// ********************************************************

// Spectral distance

// ********************************************************
// ********************************************************



int spectral_dist(int i, int j, int p) {
  if ( i>=p || j>=p || i<0 || j<0) 
    {
      printf("Index error in spectral distance: (%d, %d, %d).\n", i, j, p);
      return 0;
    }
  else
    {
      int d = abs(j-i);
      return MIN (d, p-d);
    }
}






// ********************************************************
// ********************************************************

// Sparse vectors

// ********************************************************
// ********************************************************



sparse_vect_t sparse_vect_new(int length, int weight) {
	sparse_vect_t h = malloc(sizeof (struct sparse_vect));

	h->length = length;
	h->weight = weight;
	h->coeff = (index_t *) malloc(weight * sizeof (index_t));

	return h;
}

void sparse_vect_free(sparse_vect_t h) {
	free(h->coeff);
	free(h);
}

void sparse_vect_copy_noalloc(sparse_vect_t copy, sparse_vect_t h) {
	if (copy->weight != h->weight)
		copy->coeff = (index_t *) realloc(copy->coeff, copy->weight * sizeof (index_t));
	memcpy(copy->coeff, h->coeff, h->weight * sizeof (index_t));
}

sparse_vect_t sparse_vect_copy(sparse_vect_t h) {
  sparse_vect_t copy = sparse_vect_new(h->length, h->length);
  memcpy(copy->coeff, h->coeff, h->weight * sizeof (index_t));
  return copy;
}

sparse_vect_t sparse_vect_from_indexarray(index_t * v, int length, int weight) {
	sparse_vect_t h = sparse_vect_new(length, weight);

	memcpy(h->coeff, v, weight * sizeof (index_t));

	return h;
}


static int indexcomp(const void *i, const void *j) {
	return (int) (*(index_t *)i - *(index_t *)j);
}


/* pick a random (sparse) binary block h of length h->length and of
	 weight h->weight in a previously allocated block */
sparse_vect_t sparse_vect_rand_noalloc(sparse_vect_t h, int (*rand_fct)()) {
	int i, j;
	index_t x, rand_perm_block[h->length];

	for (i = 0; i < h->length; ++i) {
		rand_perm_block[i] = i;
	}

	for (i = 0; i < h->weight; ++i) {
		j = i + rand_fct() % (h->length - i);
		x = rand_perm_block[i];
		rand_perm_block[i] = rand_perm_block[j];
		rand_perm_block[j] = x;
	}

	qsort(rand_perm_block, h->weight, sizeof (index_t), indexcomp);
	memcpy(h->coeff, rand_perm_block, h->weight * sizeof (index_t));

	return h;
}


/* allocate and pick a random (sparse) binary block of given length
	 and weight */
sparse_vect_t sparse_vect_rand(int length, int weight, int (*rand_fct)()) {
	return sparse_vect_rand_noalloc(sparse_vect_new(length, weight), rand_fct);
}



/* compute the xor of two blocks in a third one, previously allocated */
sparse_vect_t sparse_vect_xor_noalloc(sparse_vect_t h, sparse_vect_t h0, sparse_vect_t h1) {
	int i, j;

	/* make sure we have enough space in the block h */
	h->coeff = realloc(h->coeff, (vect_weight(h0) + vect_weight(h1)) * sizeof (index_t));
	i = 0;
	j = 0;
	while (1) {
		if (j == vect_weight(h1)) {
			for (; i < vect_weight(h0); ++i) {
				h->coeff[h->weight] = vect_coeff(h0, i);
				h->weight++;
			}
			break;
		} else if (i == vect_weight(h0)) {
			for (; j < vect_weight(h1); ++j) {
				h->coeff[h->weight] = vect_coeff(h1, j);
				h->weight++;
			}
			break;
		} else if (vect_coeff(h0, i) == vect_coeff(h1, j)) {
			++i;
			++j;
		} else if (vect_coeff(h0, i) > vect_coeff(h1, j)) {
			h->coeff[h->weight] = vect_coeff(h1, j);
			h->weight++;
			++j;
		} else { // sparse_vect_index(h0, i) < sparse_vect_index(h1, j)
			h->coeff[h->weight] = vect_coeff(h0, i);
			h->weight++;
			++i;
		}
	}
	/* reajust the allocated size */
	h->coeff = realloc(h->coeff, h->weight * sizeof (index_t));

	return h;
}


/* compute the xor of two blocks return the result in a newly
	 allocated block */
sparse_vect_t sparse_vect_xor(sparse_vect_t h0, sparse_vect_t h1) {
	return sparse_vect_xor_noalloc(sparse_vect_new(h0->length, 0), h0, h1);
}
















// ********************************************************
// ********************************************************

// Dense vectors

// ********************************************************
// ********************************************************


dense_vect_t dense_vect_new(int length) {
	dense_vect_t synd = malloc(sizeof (struct dense_vect));

	synd->length = length;
	synd->weight = 0;
	synd->coeff = (char *) calloc(length, sizeof (char));

	return synd;
}

void dense_vect_free(dense_vect_t synd) {
	free(synd->coeff);
	free(synd);
}

void dense_vect_copy_noalloc(dense_vect_t copy, dense_vect_t s) {
	copy->weight = s->weight;
	memcpy(copy->coeff, s->coeff, s->length * sizeof (char));
}

dense_vect_t dense_vect_copy(dense_vect_t s) {
	dense_vect_t copy = dense_vect_new(s->length);
	copy->weight = s->weight;
	memcpy(copy->coeff, s->coeff, s->length * sizeof (char));
	return copy;
}
