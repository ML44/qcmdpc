#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

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
	  printf( "%d,",g->vertices->index[ns->node[i]]);
       	}
	printf("]\n");
}




