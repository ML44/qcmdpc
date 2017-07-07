#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int main( int argc, char *argv[] ) {
  graph_t g = graph_new();
  graph_findMaxClique(g);
  return 0;
}
