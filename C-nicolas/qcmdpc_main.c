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
  int t, se, sH, arg_count, r, i, N, num_inter;
	qcmdpc_t H;
	qcblock_t e;
	spectre_t s, sh;
	compteur_t c, ctot;
	int stat_iter[MAX_ITER + 1];

	arg_count = 0;
	t = atoi(argv[++arg_count]);
	r = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
	sH = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
	se = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;

	N = r;

	for (i = 0; i <= MAX_ITER; ++i) {
		stat_iter[i] = 0;
	}

	s = spectre_new(BLOCK_LENGTH);
	sh = spectre_new(BLOCK_LENGTH);
	c = compteur_new(BLOCK_LENGTH);
	ctot = compteur_new(BLOCK_LENGTH);

	
	while (r--) {
#ifdef VERBOSE
		printf("#%d,%d %d %d %d ", sH, se, BLOCK_LENGTH, BLOCK_WEIGHT, t);
#endif
		mysrnd(sH);
		H = qcmdpc_rand(BLOCK_LENGTH, BLOCK_WEIGHT, myrnd);
		make_spectre_of(sh,qcmdpc_block(H,0));
		
		mysrnd(se);
		e = qcblock_rand(INDEX * BLOCK_LENGTH, t, myrnd);
		i = qcmdpc_decode(H, e);
		stat_iter[i]++;
		
		make_spectre_of(s,e);

		num_inter = 0;
		for (int k = 0; k<spectre_length(s); k++) 
		  {
		    if ((spectre_coeff(s,k)>0) && (spectre_coeff(sh,k)>0))
		      {
			num_inter +=1;
		      }
		    
		  }

		compteur_add_coeff(c,num_inter,i);
		compteur_add_coeff(ctot,num_inter,1);
		
		
#ifdef VERBOSE
		printf("\n");
#endif

		se++;
		sH+=2;
		
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
    fprintf(fp, "%d \t %f \t %d \n", i, (double) compteur_coeff(c,i) / ( (double)  compteur_coeff(ctot,i)), compteur_coeff(ctot,i));
  }
  fclose(fp);

  /* FILE *fpsh; */
  /* fpsh = fopen("./dat/1.sh", "w+"); */
  /* fprintf(fpsh, "gnuplot -e \"s = %f; s0 = %f; s1 = %f; s2 = %f; s3 = %f; s4 = %f; mytitle = 'Average decryption round number per distance (2 block, %d tries)'\" gnuplot-instructions.gnu > ./dat/1.png", s, sigma(0,p,w,t), sigma(1,p,w,t), sigma(2,p,w,t), sigma(3,p,w,t), sigma(4,p,w,t), N); */
  /* fclose(fpsh); */
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
