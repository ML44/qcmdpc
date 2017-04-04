#include "qcmdpc_attack.h"

char adjacence(int i,int j,qcsynd_t spectrum);
qclist_t get_neighbours(int v, qcblock_t s, qcsynd_t spectrum);
int get_p1(qcsynd_t spectrum);
qclist_t construct_A(qcsynd_t spectrum, int p1);
qclist_t construct_B(qcsynd_t spectrum, qclist_t A, int p2);
qclist_list_t get_cliques(qclist_t C, qcsynd_t spectrum, int weight);
qclist_list_t dsr(qcsynd_t spectrum, int weight);
