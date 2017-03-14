#include <stdlib.h>
#include <math.h>
/*
double lbino(int n, int t) {
  if ((t == 0) || (n == t))
    return 0.0;
  else
    return lbino(n - 1, t - 1) + log(n) - log(t);
}
*/
double lbino(int n, int t) {
  if ((t == 0) || (n == t))
    return 0.0;
  else
    return lgamma(n + 1) - lgamma(t + 1) - lgamma(n - t + 1);
}

double Euh_log(int n, int w, int t, int i) {
	return lbino(w, i) + lbino(n - w, t - i) - lbino(n, t);
}

double iks(int n, int w, int t) {
	int i;
	double x;

	for (x = 0, i = 3; (i < 10) && (i < t); i += 2) {
		x += (i - 1) * exp(Euh_log(n, w, t, i));
	}

	return x * n / 2;
}

double wsynd(int n, int w, int t) {
	int i;
	double x;

	for (x = 0, i = 1; (i < 10) && (i < t); i += 2) {
		x += exp(Euh_log(n, w, t, i));
	}

	return x * n / 2;
}

int inv_wsynd(int n, int w, double S) {
	int tmoins, t, tplus;

	tmoins = round((S * 2) / w);
	while (wsynd(n, w, tmoins) > S) {
		tmoins /= 2;
	}
	tplus = 2 * tmoins;
	while (wsynd(n, w, tplus) < S) {
		tplus *= 2;
	}

	while (tplus - tmoins > 1) {
		t = (tmoins + tplus) / 2;
		if (wsynd(n, w, t) < S)
			tmoins = t;
		else
			tplus = t;
	}
	return tplus;
}

/* Given an QC-MDPC code of index 2, length n, row weigth w. For a
	 given instance of the bit flipping decoding with syndrome weight S
	 and error weight t, returns the best threshold, i.e. the threshold
	 which minimizes the average error weight after the flips.
 */
int best_threshold_error(int n, int w, int S, int t) {
	double x, p, q, z;

	x = iks(n, w, t);

	p = ((w - 1) * S - x) / (n - t) / w * 2;
	q = (S + x) / t / w * 2;
	if (q >= 1.0)
		return w / 2;

	z = log((1 - p) / (1 - q));
	z = (log((n - t) / t) + (w / 2) * z) / (log(q / p) + z);

	return ceil(z);
}

/* Estimate the best threshold. Can use t only if iter=0.
	 Here we no not use counter[], by we could.
 */
int best_threshold(int n, int w, int S, unsigned char * counter, int iter, int t) {
	if (iter > 0) {
		t = inv_wsynd(n, w, S);
	}
	return best_threshold_error(n, w, S, t);
}

/* In a QC-MDPC code of index 2, block size p (code length 2*p), block
	 d (row weigth 2*d), when the syndrome has weight S, the counter
	 values (for non erroneous positions) follow a binomial distribution
	 of d trials of probability (close to) S/p. This fonction returns
	 the smallest counter value reached by less than one (non erroneous)
	 position on average. This is not meant to give a precise value but
	 just a probable upper bound for the best threshold value of a
	 QC-MDPC bit-flipping decoder. */
int nofail_threshold(int p, int d, int S) {
	double x, y, z;
	int T;

	x = ((double) S) / p; // average value of a syndrome bit
	T = x * d; // average value of all counters

	// expected number of position with counter value T
	y = 2 * p * exp(lbino(d, T) + T * log(x) + (d - T) * log (1 - x));
	z = x / (1 - x);

	while (y > 1) {
		// expected number of position with counter value T+1
		// y <- y * (d - T) / (T + 1) * x / (1 - x)
		y *= (d - T) * z;
		y /= (T + 1);
		++T;
	}

	return T;
}

/* As above, but tries to guess a lower bound for the threshold.
	 The lower bound is (arbitrarily) fixed to the threshold for which
	 sqrt(p) positions are flipped or f_max if > 0. */
int fail_threshold(int p, int d, int S, int f_max) {
	double x, y, z, u;
	int T;

	x = ((double) S) / p; // average value of a syndrome bit
	T = x * d; // average value of all counters

	if (f_max > 0)
		u = f_max;
	else
		u = sqrt((double) p);

	// expected number of position with counter value T
	y = 2 * p * exp(lbino(d, T) + T * log(x) + (d - T) * log (1 - x));
	z = x / (1 - x);

	while (1) {
		// expected number of position with counter value T+1
		// y <- y * (d - T) / (T + 1) * x / (1 - x)
		y *= (d - T) * z;
		y /= (T + 1);
		if (y < u)
			break; // before we increment T
		++T;
	}

	return T;
}

int * fail_nofail_threshold(int p, int d, int S) {
	double x, y, z, u;
	int * T = (int *) malloc(2 * sizeof (int));

	x = ((double) S) / p; // average value of a syndrome bit
	T[1] = x * d; // average value of all counters
	u = sqrt((double) p);

	// expected number of position with counter value T
	y = 2 * p * exp(lbino(d, T[1]) + T[1] * log(x) + (d - T[1]) * log (1 - x));
	z = x / (1 - x);

	T[0] = 0;
	while (y > 1) {
		// expected number of position with counter value T+1
		// y <- y * (d - T) / (T + 1) * x / (1 - x)
		y *= (d - T[1]) * z;
		y /= (T[1] + 1);
		if ((T[0] == 0) && (y < u)) {
			T[0] = T[1];
		}
		++T[1];
	}

	return T;
}
