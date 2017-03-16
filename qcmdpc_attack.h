#include "qcmdpc.h"

#define qcsynd_set_coeff(synd, i) ((synd)->coeff[i] = 1)
typedef uint32_t dist_count_t;
struct node {
  index_t val;
  struct node * next;
};
typedef struct node * node_t;
struct list {
  int    length;
  int    weight;
  node_t index;
};
typedef struct list * list_t;

dist_count_t * dist_count_new(int length);
qcsynd_t dist_spectre(qcblock_t e);
void dist_count_add_spectre(dist_count_t (* counter), qcsynd_t spectre, int ws);
void dist_count_print(dist_count_t (* counter), int p, char * str);
void qcsynd_print(qcsynd_t s, char * str);
void qcsynd_print2(qcsynd_t s1, qcsynd_t s2, int l);
void qcsynd_compare(qcsynd_t s1, qcsynd_t s2, int l);
void qcblock_print(qcblock_t h, char * str);
void dist_count_free(dist_count_t (* counter));
qcblock_t qcblock_or_noalloc(qcblock_t h, qcblock_t h0, qcblock_t h1);
int dist_count_mean(dist_count_t * counter, int p);
qcsynd_t dist_spectre_reconstruct(dist_count_t * counter, int p, int m);
char dist_test(qcsynd_t s, list_t k, int i);
char dist_reconstruct_aux(qcsynd_t spectre, list_t k, int w);
qcblock_t dist_reconstruct(qcsynd_t spectre, int w);
void qcblock_add(qcblock_t k, int i);
void qcblock_remove(qcblock_t k, int i);

#define list_weight(l) ((l)->weight)
#define list_length(l) ((l)->length)
list_t list_init(int len);
char list_isempty(list_t l);
void list_add(list_t l, index_t v);
void list_remove(list_t l);
void list_print(list_t l, char * str);
list_t list_from_qcblock(qcblock_t h);
qcblock_t qcblock_from_list(list_t l);
