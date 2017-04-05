#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "graphs.h"



// ********************************************************
// ********************************************************

// Node Set

// ********************************************************
// ********************************************************



nodeSet_t nodeSet_new ( void ) {
  nodeSet_t ns = malloc( sizeof ( struct nodeSet ) );
  ns->N = 0;
  ns->size = 0;
  return ns;
}

void nodeSet_free( nodeSet_t ns ) {
  free( ns );
}

nodeSet_t nodeSet_init( int NInput ) {
  nodeSet_t ns = malloc( sizeof ( struct nodeSet ) );
  ns->N = NInput;
  ns->size = 0;
  ns->node = (int *) malloc( NInput*sizeof(int) );
  return ns;
}

void nodeSet_add( nodeSet_t ns, int nodeA ) {
  ns->node[ns->size++] = nodeA;
}

void nodeSet_remove( nodeSet_t ns ) {
  ns->size--;
}

void nodeSet_print( graph_t g, nodeSet_t ns ) {
	int i;
	printf(" the size of clique : %d - [",ns->size);
	for ( i = 0; i < ns->size; i++ ) {
	  printf( "%d,",g->vertices->coeff[ns->node[i]]);
       	}
	printf("]\n");
}


list_t nodeSet_to_list( graph_t g, nodeSet_t ns ) {
  list_t l = list_init(g->p);
  for (int i = ns->size - 1; i >= 0; i-- ) {
    list_add(l, g->vertices->coeff[ns->node[i]]);
  }
  return l;
}






// ********************************************************
// ********************************************************

// Graphs

// ********************************************************
// ********************************************************



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
    printf( "%d ; ",g->vertices->coeff[i]);
  }
  printf("} \n");
}

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

char graph_areConnected( graph_t g, int nodeA, int nodeB ) {
  int i = g->vertices->coeff[nodeA];
  int j = g->vertices->coeff[nodeB];
  int d = (j>i) ? (MIN(j-i,(g->p)-(j-i))) : (MIN(i-j,(g->p)-(i-j))); // TODO define higher function

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






// ********************************************************
// ********************************************************

// Find Maximal Cliques (BK2)

// ********************************************************
// ********************************************************


// An implementation of Bron-Kerbosch algorithm
// From Algorithm 457 of the Collected Algorithms from CACM
// http://www.netlib.org/tomspdf/457.pdf
list_list_t graph_findMaxClique( graph_t g ) {
  int i;
  int *all = (int *) malloc(g->N*sizeof(int));
  for (i=0; i<g->N; i++) {
    all[i] = i;
  }
  list_list_t result_list = list_list_init();
  graph_bkv2( g, all, 0, g->N, result_list );
  free( all );
  return result_list;
}

// recursive function version 2 of Bron-Kerbosch algorithm
void graph_bkv2( graph_t g, int* oldSet, int ne, int ce, list_list_t l) {
  int *newSet = (int *)malloc(ce*sizeof(int));
  int nod, fixp;
  int newne, newce, i, j, count, pos, p, s, sel, minnod;

  minnod = ce;
  nod = 0;
  // Determine each counter value and look for minimum
  for ( i = 0 ; i <ce && minnod != 0; i++) {
    p = oldSet[i];
    count = 0;
    // Count disconnections
    for (j = ne; j < ce && count < minnod; j++)
      if (!graph_areConnected(g,p,oldSet[j])) {
	count++;
	// Save position of potential candidate
	pos = j;
      }
    // Test new minimum
    if (count < minnod) {
      fixp = p;
      minnod = count;
      if (i<ne)
	s = pos;
      else {
	s = i;
	// pre-increment
	nod = 1;
      }
    }
  }
  
  // If fixed point initially chosen from candidates then
  // number of diconnections will be preincreased by one
  // Backtrackcycle
  for (nod=minnod+nod; nod>=1; nod--) {
    // Interchange
    p = oldSet[s];
    oldSet[s] = oldSet[ne]; 
    sel = oldSet[ne] = p;
    // Fill new set "not"
    newne = 0;
    for ( i = 0 ; i < ne ; i++)
      if ( graph_areConnected(g,sel,oldSet[i]) )
	newSet[newne++] = oldSet[i];
    
    // Fill new set "cand"
    newce = newne;
    for (i=ne+1; i<ce; i++)
      if ( graph_areConnected(g,sel,oldSet[i]) )
	newSet[newce++] = oldSet[i];
    
    // Add to compsub
    nodeSet_add(g->subgraph, sel );
    if (newce == 0) {
      // found a max clique
      list_list_add(l, nodeSet_to_list(g, g->subgraph)); // TODO stocker et retourner la liste
      
    } else if (newne < newce)
      graph_bkv2(g, newSet, newne, newce, l);
    
    // Remove from subgraph
    nodeSet_remove(g->subgraph);

    // Add to "not"
    ne++;
    if (nod > 1) {
      // Select a candidate disconnected to the fixed point
      for ( s = ne ; graph_areConnected(g,fixp,oldSet[s]) ; s++)
	{
	  ;
	}
      // nothing but finding s
      
    } /* Backtrackcycle */
    /* free( newSet ); */ // TODO verifier desallocation memoire
  } 
}
