#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "graph.h"

graph_t graph_new ( qcblock_t vertices, qcsynd_t spectrum ) {

  graph_t g = malloc ( sizeof ( struct graph ) );
  g->p = vertices->length; // a changer pour plusieurs blocs
  g->N = vertices->weight;
  g->vertices = vertices;
  g->spectrum = spectrum;
  g->subgraph = nodeSet_init( vertices->weight );
  
  return g;
}


void graph_free( graph_t g ) {
  qcblock_free( g->vertices );
  qcsynd_free( g->spectrum );
  nodeSet_free( g->subgraph );
  free(g);
}

void graph_print( graph_t g ) {
  printf("graph = { ");
  for (int i=0; i<(g->N); i++) {
    printf( "%d ; ",g->vertices->index[i]);
  }
  printf("} \n");
}

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int min_dist( int i, int j, int p) {
  if (j>i) {
    return MIN(j-i,p-(j-i));
  }
  else {
    return MIN(i-j,p-(i-j));
  }
}


char graph_areConnected( graph_t g, int nodeA, int nodeB ) {
  int i = g->vertices->index[nodeA];
  int j = g->vertices->index[nodeB];
  int d = min_dist(i,j,g->p);
  /* printf("Check connected %d %d (dist %d) : ", i, j, d); */
  if ( nodeA >= g->N || nodeB >= g->N ) {
    printf( "AccessViolationInNodeConnectionCheck\n");
    exit(-1);
  }
  char b = (d==0) || g->spectrum->coeff[d-1];
  /* if (b) */
  /*   { printf("TRUE \n"); } */
  /* else */
  /*   { printf("FALSE \n"); } */
  return b;
}

int graph_size( graph_t g ) { 
  return g->N;
}

