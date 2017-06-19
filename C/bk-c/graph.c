#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "graph.h"

graph_t graph_new ( void ) { //make a random test pattern;
  int i,j;
  srand( (unsigned int) time( NULL ) );
  int N = (int) (rand()%6)+3; // the maximum number of nodes : 8
  char ** connected = (char **) malloc( N*sizeof(char *) );
  for ( i = 0; i < N; i++)
    connected[i] = (char *) malloc( N*sizeof(char) );
  for ( i = 0; i < N; i++) { // the graph is randomly generated
    connected[i][i] = 1;
    for ( j = i+1; j < N; j++ ) {
      if ( rand()%2 == 1 ) {
	connected[i][j] = 1;
	connected[j][i] = 1;
      }
      else {
	connected[i][j] = 0;
	connected[j][i] = 0;
      }
    }
  }

  graph_t g = malloc ( sizeof ( struct graph ) );
  g->N = N;
  g->connected = connected;
  g->compsub = nodeSet_init( N );
  
  graph_print(g);

  return g;
}

graph_t graph_new_from_file ( char* fileName ) { // make a graph from a file
  char c[1000];
  int i = 0;
  int row = 0;
  int col,j;
  FILE* fp;
  int N = 0;
  char ** connected;
  if ( ( fp = fopen(fileName, "rt") ) == NULL ) {
    printf( "FileOpenError!\n" );
    exit(-1);
  }
  while (!feof(fp)) {
    fgets( c, 1000, fp );
    if ( N == 0 ) {
      i = 0;
      while( c[i] != '\n' ) {
	if ( c[i]=='0' || c[i]=='1' )
	  N++;
	i++;
      }
      if ( N == 0 ) {
	printf( "TheFileIsCorrupted!\n");
	exit(-1);
      }
      connected = (char **) malloc( N*sizeof(char *) );
    }
    if ( row < N )
      connected[row] = (char *) malloc( N*sizeof(char) );
    col = 0; 
    i = 0;
    while( c[i] != '\n' ) {
      if ( c[i]=='0') {
	if ( col>=N || row >= N) {
	  printf( "TheFileIsCorrupted!\n");
	  exit(-1);
	}
	connected[row][col++] = 0;
      }
      if ( c[i]=='1') {
	if ( col>=N || row >= N) {
	  printf( "TheFileIsCorrupted!\n");
	  exit(-1);
	}
	connected[row][col++] = 1;
      }
      i++;
    }
    row++;
  }
  fclose( fp );

  graph_t g = malloc ( sizeof ( struct graph ) );
  g->N = N;
  g->connected = connected;
  g->compsub = nodeSet_init( N );

  graph_print(g);

  return g;
}

void graph_free( graph_t g ) {
  int i;
  for ( i = 0; i < g->N; i++ )
    free( g->connected[i] );
  free( g->connected );
  free(g);
}

char graph_areConnected( graph_t g, int nodeA, int nodeB ) {
  if ( nodeA >= g->N || nodeB >= g->N ) {
    printf( "AccessViolationInNodeConnectionCheck\n");
    exit(-1);
  }
  return g->connected[nodeA][nodeB];
}

int graph_size( graph_t g ) { 
  return g->N;
}

void graph_print( graph_t g ) {
  int i,j;
  printf("\n");
  for ( i = 0; i < g->N; i++ ) {
    for ( j = 0; j < g->N; j++ ) {
      if (g->connected[i][j])
	printf("1");
      else printf("0");
    }
    printf("\n");
  }
  printf("\n");
} 

