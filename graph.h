#include "qcmdpc.h"

struct nodeSet {
  int N;
  int *node;
  int size;
};
typedef struct nodeSet * nodeSet_t;

nodeSet_t nodeSet_new ( void );
void nodeSet_free ( nodeSet_t ns );
nodeSet_t nodeSet_init( int NInput );
void nodeSet_add( nodeSet_t ns, int nodeA );
void nodeSet_remove( nodeSet_t ns );


// TODO changer les noms ?
struct graph {
  int p; // the size of the code
  int N; // the size of the graph
  qcblock_t vertices;
  qcsynd_t spectrum;
  nodeSet_t subgraph;
};
typedef struct graph * graph_t;

graph_t graph_new (  qcblock_t vertices, qcsynd_t spectrum ); // default constructor;
void graph_free ( graph_t g );
void graph_print ( graph_t g );
char graph_areConnected( graph_t g, int nodeA, int nodeB ); // are two nodes connected?
int graph_size( graph_t g );
void graph_bkv2(graph_t g, int *old, int ne, int ce);
void graph_findMaxClique( graph_t g );

void nodeSet_print( graph_t g, nodeSet_t ns );
