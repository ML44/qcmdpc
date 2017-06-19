#include "attack.h"
#include "graphs.h"

// Reconstruction functions of first paper
char spectrum_test_new_bit(qcsynd_t s, list_t k, int i);
char block_from_spectrum_aux(qcsynd_t spectre, list_t k, int w, int b);
qcblock_t block_from_spectrum(qcsynd_t spectre, int w);
void block_reconstruction(int length, int weight, int seed);


// Reconstruction functions of second paper
char adjacence(int i,int j,qcsynd_t spectrum);
list_t get_neighbours(int v, qcblock_t s, qcsynd_t spectrum);
int get_p1(qcsynd_t spectrum);
list_t construct_A(qcsynd_t spectrum, int p1);
list_t construct_B(qcsynd_t spectrum, list_t A, int p2);
list_list_t get_cliques(list_t C, qcsynd_t spectrum, int weight);
list_list_t dsr(qcsynd_t spectrum, int weight);
