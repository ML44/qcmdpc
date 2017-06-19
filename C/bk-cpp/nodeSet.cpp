#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

nodeSet::nodeSet( void ) {
	N = 0;
	size = 0;
}

nodeSet::~nodeSet( void ) {
	if ( N != 0 )
		free( node );
}

void nodeSet::nodeInit( int NInput ) {
	N = NInput;
	node = (int *) malloc( N*sizeof(int) );
	size = 0;
}

void nodeSet::add( int nodeA ) {
	node[size++] = nodeA;
}

void nodeSet::remove( void ) {
	size--;
}

void nodeSet::print( void ) {
	int i;
	printf(" the size of clique : %d - [",size);
	for ( i = 0; i < size; i++ ) {
		if ( i < size-1 )
			printf( "%d,",node[i]);
		else printf( "%d ]\n",node[i]);
	}
}




