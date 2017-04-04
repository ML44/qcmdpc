# include "lib.h"

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
// new empty qclist
// -----------------------------------
qclist_list_t qclist_list_init(int len) {
  qclist_list_t l = malloc( sizeof ( struct qclist_list ) );
  l->index = NULL;
  l->weight=0;
  l->length=len;
  return l;
}



// -----------------------------------
// tests if qclist is empty
// -----------------------------------
char qclist_list_isempty(qclist_list_t l) {
  if (l->index == NULL) {return 1;} return 0;
}



// -----------------------------------
// adds value v at the beginning of the qclist
// -----------------------------------
void qclist_list_add(qclist_list_t l, qclist_t v) {
  qclist_node_t head = malloc(sizeof(qclist_node_t));
  head->val = v;
  head->next = l->index;
  l->index = head;
  l->weight++;
}



// -----------------------------------
// removes the first value of the qclist
// -----------------------------------
void qclist_list_remove(qclist_list_t l) {
  if(l->weight>0) {
    l->index = l->index->next;
    l->weight--;
  }
}



// -----------------------------------
// prints the qclist with str prefix message
// -----------------------------------
void qclist_list_print(qclist_list_t l, char * str) {
  printf("%s = { \n", str);
  qclist_node_t current = l->index;
  while (current != NULL) {
    qclist_print(current->val, "");
    current = current->next;
  }
  printf("}\n");
}
