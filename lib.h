#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))


typedef uint16_t index_t; // limits the code length to 65536
//typedef uint32_t index_t;

struct node {
  index_t val;
  struct node * next;
};
typedef struct node * node_t;
struct qclist {
  int    length;
  int    weight;
  node_t index;
};
typedef struct qclist * qclist_t;
#define qclist_weight(l) ((l)->weight)
#define qclist_length(l) ((l)->length)
qclist_t qclist_init(int len);
char qclist_isempty(qclist_t l);
void qclist_add(qclist_t l, index_t v);
void qclist_remove(qclist_t l);
void qclist_print(qclist_t l, char * str);





struct qclist_node {
  qclist_t val;
  struct qclist_node * next;
};
typedef struct qclist_node * qclist_node_t;
struct qclist_list {
  int    length;
  int    weight;
  qclist_node_t index;
};
typedef struct qclist_list * qclist_list_t;
#define qclist_list_weight(l) ((l)->weight)
#define qclist_list_length(l) ((l)->length)
qclist_list_t qclist_list_init(int len);
char qclist_list_isempty(qclist_list_t l);
void qclist_list_add(qclist_list_t l, qclist_t v);
void qclist_list_remove(qclist_list_t l);
void qclist_list_print(qclist_list_t l, char * str);
