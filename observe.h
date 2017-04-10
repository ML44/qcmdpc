#include "attack.h"

long long combi(int n, int k);
float average_syndrom_weight(int n, int w, int t);


// Spectrum reconstruction
void spectrum_reconstruction(int p, int w, int t, int N, int se, int sh);
qcsynd_t observe_spectrum(qcmdpc_t H, int p, int w, int t, int N, int se, float threshold);

// Data functions
void syndrom_weight_distribution(int p, int w, int t, int d, int N, int se, int sh);
