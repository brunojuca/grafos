#include <Graph.h>
#include <Edge.h>
#include <Node.h>
#include <iostream>

int main()
{
    Graph grafo(4, false, false, false);

    grafo.insertNode(4, 0);
    grafo.insertNode(1, 0);
    grafo.insertNode(2, 0);
    grafo.insertNode(3, 0);
    grafo.insertNode(5, 0);
    grafo.insertNode(0, 0);

    grafo.insertEdge(1, 2, 0);
    grafo.insertEdge(1, 0, 0);
    grafo.insertEdge(1, 3, 0);
    grafo.insertEdge(3, 2, 0);
    grafo.insertEdge(3, 5, 0);

    grafo.generateDot("teste");
    Graph *newGraph = grafo.transitiveClosure(2);

    newGraph->generateDot("Teste_transitive_closure");
}