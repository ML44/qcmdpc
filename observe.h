#include "attack.h"

// Spectrum reconstruction
void spectrum_reconstruction(int p, int w, int t, int N, int se, int sh);
qcsynd_t observe_spectrum(qcmdpc_t H, int p, int w, int t, int N, int se, float threshold);

// Data functions
void syndrom_weight_distribution(int p, int w, int t, int d, int N, int se, int sh);
