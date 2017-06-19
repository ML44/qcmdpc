#ifndef GRAPH_H
#define GRAPH_H

class nodeSet {
 private:
  int N;
  int *node;
  int size;
 public:
  nodeSet( void );
  ~nodeSet( void );
  void nodeInit( int NInput );
  void add( int nodeA );
  void remove( void );
  void print( void );
};


class Graph {
 private:
  int N; // the number of node
  char **connected;
  nodeSet compsub;
 public:
  Graph(); // default constructor;
  Graph( char* fileName ); // constructor with file
  ~Graph();
  char areConnected( int nodeA, int nodeB ); // are two nodes connected?
  int size( void );
  void findMaxClique( void );
  void print( void );
 private:
  void bkv2(int *old, int ne, int ce);
};

#endif 
