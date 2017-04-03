
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
void nodeSet_print( nodeSet_t ns );


// TODO changer les noms ?
struct graph {
  int N; // the number of node
  char **connected;
  nodeSet_t compsub;
};
typedef struct graph * graph_t;

graph_t graph_new ( void ); // default constructor;
graph_t graph_new_from_file ( char* fileName ); // constructor with file
void graph_free ( graph_t g );
char graph_connected( graph_t g, int nodeA, int nodeB ); // are two nodes connected?
int graph_size( graph_t g );
void graph_print( graph_t g );
void graph_bkv2(graph_t g, int *old, int ne, int ce);
void graph_findMaxClique( graph_t g );
