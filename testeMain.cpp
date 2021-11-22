#include <Graph.h>
#include <Edge.h>
#include <Node.h>
#include <iostream>

int main()
{
    Graph grafo(7, true, false, false);

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
    
    Graph grafo2(7, true, false, false);

    grafo2.insertNode(0, 0);
    grafo2.insertNode(1, 0);
    grafo2.insertNode(2, 0);
    grafo2.insertNode(3, 0);
    grafo2.insertNode(4, 0);
    grafo2.insertNode(5, 0);
    grafo2.insertNode(6, 0);

    grafo2.insertEdge(0, 1, 0);
    grafo2.insertEdge(0, 2, 0);
    grafo2.insertEdge(0, 3, 0);
    grafo2.insertEdge(1, 2, 0);
    grafo2.insertEdge(4, 1, 0);
    grafo2.insertEdge(2, 5, 0);
    grafo2.insertEdge(5, 4, 0);

    grafo2.generateDot("grafo2");

    grafo2.breadthFirstSearch(0)->generateDot("grafo2_BFS");
}