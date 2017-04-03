#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

// An implementation of Bron-Kerbosch algorithm
// From Algorithm 457 of the Collected Algorithms from CACM
// http://www.netlib.org/tomspdf/457.pdf
void graph_findMaxClique( graph_t g ) {
  int i;
  int *all = (int *) malloc(g->N*sizeof(int));
  for (i=0; i<g->N; i++) {
    all[i] = i;
  }
  graph_bkv2( g, all, 0, g->N );
  free( all );
}

// recursive function version 2 of Bron-Kerbosch algorithm
void graph_bkv2( graph_t g, int* oldSet, int ne, int ce ) {
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
      nodeSet_print(g, g->subgraph); // TODO stocker et retourner la liste
      
    } else if (newne < newce)
      graph_bkv2(g, newSet, newne, newce );
    
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
