#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "qcmdpc_dec.h"

long random(void);
void srandom(unsigned int seed);
int myrnd() { return random(); }
void mysrnd(int seed) { srandom(seed); }

int main(int argc, char ** argv) {
        int t, se, sH, arg_count, r, i, N;
	qcmdpc_t H;
	qcblock_t e;
	spectre_t s;
	compteur_t c, ctot;
	int stat_iter[MAX_ITER + 1];

	arg_count = 0;
	t = atoi(argv[++arg_count]);
	r = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
	sH = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
	se = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;

	N = r;
	mysrnd(sH);
	H = qcmdpc_rand(BLOCK_LENGTH, BLOCK_WEIGHT, myrnd);

	for (i = 0; i <= MAX_ITER; ++i) {
		stat_iter[i] = 0;
	}

	s = spectre_new(BLOCK_LENGTH);
	c = compteur_new(BLOCK_LENGTH);
	ctot = compteur_new(BLOCK_LENGTH);
	
	while (r--) {
#ifdef VERBOSE
		printf("#%d,%d %d %d %d ", sH, se, BLOCK_LENGTH, BLOCK_WEIGHT, t);
#endif
		mysrnd(se);
		e = qcblock_rand(INDEX * BLOCK_LENGTH, t, myrnd);
		i = qcmdpc_decode(H, e);
		stat_iter[i]++;

		// TODO . filtrer i (2 3 4 5)
		
		make_spectre_of(s,e);
		compteur_add_spectrum(c, s, i);
		compteur_add_spectrum(ctot, s, 1);
		
		/* printf("e = "); */
		/* for(int i=0; (i<qcblock_weight(e)-1) & (qcblock_index(e,i) < BLOCK_LENGTH); ++i) */
		/*   { */
		/*     printf("%d ; ", qcblock_index(e,i)); */
		/*   } */
		/* printf("\n"); */

		/* printf("s = "); */
		/* for(int k=0; k<spectre_length(s); ++k) */
		/*   { */
		/*     printf("%d ; ", spectre_coeff(s,k)); */
		/*   } */
		/* printf("\n"); */
		
		/* qcblock_free(e); */

		
#ifdef VERBOSE
		printf("\n");
#endif

		se++;
	}
	
  make_spectre_of(s,qcmdpc_block(H,0));

  for (int i=0; i<compteur_length(ctot); ++i)
    {
      if ((compteur_coeff(ctot, i)==0))
  	{
  	  compteur_coeff(ctot, i) += 1;
  	}
    }
  
  
  
  FILE *fp;
  fp = fopen("./out.dat", "w+");
  fprintf(fp, "#Nombre d'essais = %d, \n#Poids de l'erreur = %d, \n#Taille d'un bloc = %d, \n#Poids de h = %d. \n", N, t, BLOCK_LENGTH, BLOCK_WEIGHT);
  for (int i=0; i<spectre_length(s); i++) {
    fprintf(fp, "%d \t %f \t %d \n", i, (double) compteur_coeff(c,i) / ( (double)  compteur_coeff(ctot,i)), spectre_coeff(s,i));
  }
  fclose(fp);  

  // call gnuplot
   /* float s = average_syndrom_weight(p,w,t); */
   /* FILE *fpsh; */
   /* fpsh = fopen("./dat/1.sh", "w+"); */
   /* fprintf(fpsh, "gnuplot -e \"s = %f; s0 = %f; s1 = %f; s2 = %f; s3 = %f; s4 = %f; mytitle = 'Average syndrom weight per distance (1 block, %d tries)'\" gnuplot-instructions.gnu > ./dat/1.png", s, sigma(0,p,w,t), sigma(1,p,w,t), sigma(2,p,w,t), sigma(3,p,w,t), sigma(4,p,w,t), N); */
   /* fclose(fpsh);   */
   /* system("sh ./dat/1.sh"); */


	
	/* printf("\n"); */

	/* printf("c = "); */
	/* for(int k=0; k<compteur_length(c); ++k) */
	/*   { */
	/*     printf("%d ; ", compteur_coeff(c,k)); */
	/*   } */
	/* printf("\n"); */


	/* printf("h = "); */
	/* for(int i=0; (i<qcblock_weight(H)-1) & (qcblock_index(H,i) < BLOCK_LENGTH); ++i) */
	/*   { */
	/*     printf("%d ; ", qcblock_index(H,i)); */
	/*   } */
	/* printf("\n"); */
		
	



	qcmdpc_free(H);
	spectre_free(s);
	compteur_free(c);
	
	

#ifndef VERBOSE
	for (i = 1; i <= MAX_ITER; ++i) {
		if (stat_iter[i] != 0) {
			printf("%d iterations:\t %d\n", i, stat_iter[i]);
		}
	}
	printf("failures (%d):\t %d\n", MAX_ITER, stat_iter[0]);
#endif


	return 0;
}
