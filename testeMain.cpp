#include <Graph.h>
#include <Edge.h>
#include <Node.h>
#include <iostream>

int main()
{
    Graph grafo(7, true, false, false);

    grafo.insertNode(4);
    grafo.insertNode(1);
    grafo.insertNode(2);
    grafo.insertNode(3);
    grafo.insertNode(5);
    grafo.insertNode(0);
    grafo.insertNode(6);

    grafo.insertEdge(1, 0);
    grafo.insertEdge(2, 0);
    grafo.insertEdge(3, 0);
    grafo.insertEdge(4, 0);
    grafo.insertEdge(5, 0);

    grafo.generateDot("Grafo_Original");

    Graph *newGraph = grafo.directTransitiveClosure(0);
    newGraph->generateDot("Teste_direct_transitive_closure");
    delete newGraph;
    newGraph = grafo.indirectTransitiveClosure(0);
    newGraph->generateDot("Teste_indirect_transitive_closure");
    delete newGraph;
    Graph grafo2(7, true, true, false);

    grafo2.insertNode(0);
    grafo2.insertNode(1);
    grafo2.insertNode(2);
    grafo2.insertNode(3);
    grafo2.insertNode(4);
    grafo2.insertNode(5);
    grafo2.insertNode(6);

    grafo2.insertEdge(0, 1);
    grafo2.insertEdge(0, 2);
    grafo2.insertEdge(0, 3);
    grafo2.insertEdge(1, 2);
    grafo2.insertEdge(4, 1, 10);
    grafo2.insertEdge(2, 5);
    grafo2.insertEdge(5, 4);

    grafo2.generateDot("grafo2");

    newGraph = grafo2.breadthFirstSearch(4);

    float distancia_teste = grafo2.floydMarshall(0,4);
    cout << "Distancia por BFS: " << distancia_teste << endl;
    delete newGraph;
}