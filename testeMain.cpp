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
    grafo.insertNode(6, 0);

    grafo.insertEdge(1, 0, 0);
    grafo.insertEdge(2, 0, 0);
    grafo.insertEdge(3, 0, 0);
    grafo.insertEdge(4, 0, 0);
    grafo.insertEdge(5, 0, 0);

    grafo.generateDot("Grafo_Original");

    Graph *newGraph = grafo.directTransitiveClosure(0);
    newGraph->generateDot("Teste_direct_transitive_closure");
    
    newGraph = grafo.indirectTransitiveClosure(0);
    newGraph->generateDot("Teste_indirect_transitive_closure");
}