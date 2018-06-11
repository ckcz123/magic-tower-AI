#include <cstring>
#include "graph.h"

Graph* graph = nullptr;

int main(int argc, char** argv) {

  bool shouldMerge = true, shouldEat = true;

  char filename[100] = "";
  int print = 1000;
  for (int i=1;i<argc;i++) {
    if (strcmp(argv[i], "--nomerge")==0) shouldMerge=false;
    else if (strcmp(argv[i], "--noeat")==0) shouldEat=false;
    else if (strcmp(argv[i], "--file")==0) {
      strcpy(filename, argv[++i]);
    }
    else if (strcmp(argv[i], "--print")==0) {
      sscanf(argv[++i], "%d", &print);
    }
  }

  FILE* file = nullptr;
  if (strlen(filename)!=0) {
    file = freopen(filename, "r", stdin);
  }
  graph = new Graph(shouldMerge, shouldEat);

  if (file!=nullptr)
    fclose(file);

  graph->setPrint(print);

  graph->initGraph();

  graph->print();

  graph->run();

  delete graph;
  return 0;

}
