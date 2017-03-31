#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int main( int argc, char *argv[] ) {
	if ( argc < 2 ) {
		Graph a = Graph(); // if no file, generate a random graph
		a.findMaxClique();
	}
	else {
		int i;
		for ( i = 1; i < argc; i++ ) {
			Graph a( argv[i] );
			a.findMaxClique();
		}
	}
	return 0;
}
