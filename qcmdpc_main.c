#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "qcmdpc_dec.h"
#include "qcmdpc_attack.h"

long random(void);
void srandom(unsigned int seed);
int myrnd() { return random(); }
void mysrnd(int seed) { srandom(seed); }

int main(int argc, char ** argv) {

/*  
  int bl, bw, ind;
  // bl = BLOCK_LENGTH;
  // bw = BLOCK_WEIGHT;
  // ind = INDEX;
  ind = 1; // TODO ADAPTER POUR ind > 1

  // arguments
  int arg_count, N, p, t, se, sH, ws, seuil, m;
  arg_count = 0;
  // longueur du bloc
  p = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 10;
  bl = p;
  // poids du polynome h (d)
  bw = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 4; 
  // nombre d'erreurs (poids de e)
  t = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 2; 
  // nombre de tests
  N = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 20;
  // seuil (si 0, seuil = m la moyenne)
  seuil = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 0; 
  // seed for e
  se = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;
  // seed for H
  sH = (argc > arg_count + 1) ? atoi(argv[++arg_count]) : 1;

  qcblock_t e, h;
  qcsynd_t synd_e, spectre_e, spectre_h, spectre_h_reconstruct;
  dist_count_t * compteur;
  qcmdpc_t H;

  printf("Nombre d'essais = %d, \nLongueur du vecteur = %d, \nPoids de l'erreur = %d, \nNombre de blocs = %d, \nTaille d'un bloc = %d, \nPoids de h = %d. \n", N, ind*p, t, ind, bl, bw);
  
  compteur = dist_count_new(p-1);
  mysrnd(sH);
  H = qcmdpc_rand(bl, bw, myrnd);
  
  while (--N) {
    //printf("\n");
    mysrnd(se);
    e = qcblock_rand(ind * bl, t, myrnd);
    synd_e = qcmdpc_synd(H, e);
    //qcblock_print(e, "e");
    //qcsynd_print(synd_e, "s");
    ws = qcsynd_weight(synd_e);
    //printf("Poids de s : %d\n", ws);
    spectre_e = dist_spectre(e);
    //qcsynd_print(spectre_e, "spectre_e");
    dist_count_add_spectre(compteur, spectre_e, ws);
    //dist_count_print(compteur, p-1, "compteur cumule");
    se++;
  }

  //printf("\n ======== \n");

  //dist_count_print(compteur, p-1, "compteur cumule");
  m = dist_count_mean(compteur, p-1);
  seuil = (seuil == 0) ? m : seuil;
  spectre_h_reconstruct = dist_spectre_reconstruct(compteur, p-1, seuil);
  //qcsynd_print(spectre_h_reconstruct, "spectre_h deduit");
  
  h = qcmdpc_block(H,0);
  spectre_h = dist_spectre(h);
  // TODO spectre cumule par bloc de h
  // spectre_h_cum = qcsynd_new(h->length, 0).
  //qcblock_print(h, "h");
  //qcsynd_print(spectre_h, "spectre_h reel  ");

  printf("moyenne = %d\n", m);
  printf("seuil = %d\n", seuil);

  //qcsynd_print2(spectre_h, spectre_h_reconstruct, p-1);
  qcsynd_compare(spectre_h, spectre_h_reconstruct, p-1);
  
  // free
  qcblock_free(e);
  qcblock_free(h);
  qcsynd_free(synd_e);
  qcsynd_free(spectre_e);
  qcsynd_free(spectre_h);
  dist_count_free(compteur);
*/


 
  qcblock_t h = qcblock_rand(10,4,myrnd);
  qcblock_print(h, "h0");
  qcsynd_t s = dist_spectre(h);
  qcsynd_print(s, "sh");
  list_t l = dist_reconstruct(s, 4);
  list_print(l, "lh");

/* TODO : reconversit list en qcblock et generer le spectre pour comparer */
  
  return 0;
}
