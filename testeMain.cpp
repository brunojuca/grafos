#include <Graph.h>
#include <Edge.h>
#include <Node.h>
#include <iostream>

Graph *createGraph(std::string graph_file_path, bool directed, bool weighted_edge, bool weighted_node)
{
    ifstream graph_file;
    graph_file.open(graph_file_path, ios::in);

    int order;
    graph_file >> order;

    Graph* graph = new Graph(order, directed, weighted_edge, weighted_node);

    int idNodeSource, idNodeTarget;


    if (!weighted_edge && !weighted_node)
    {

        while (graph_file >> idNodeSource >> idNodeTarget)
        {
            graph->insertEdge(idNodeSource, idNodeTarget, 1);
        }
    }
    else if (weighted_edge && !weighted_node)
    {

        float edgeWeight;

        while (graph_file >> idNodeSource >> idNodeTarget >> edgeWeight)
        {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
        }
    }
    else if (weighted_node && !weighted_edge)
    {

        float nodeSourceWeight, nodeTargetWeight;

        while (graph_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight)
        {

            graph->insertEdge(idNodeSource, idNodeTarget, 1);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);
        }
    }
    else if (weighted_node && weighted_edge)
    {

        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while (graph_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight)
        {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);
        }
    }
    
    return graph;
}

// int main(char* graph_file_path, bool directed, bool weighted_edge, bool weighted_node)
int main(int argsc, char* args[])
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
    Graph grafo2(7, false, true, false);

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

    newGraph = grafo2.floydMarshall(4, 3);
    newGraph->generateDot("Menor_Caminho");

    delete newGraph;

    newGraph = createGraph(args[1], stoi(args[2]), stoi(args[3]), stoi(args[4]));
    
    std::cout << "Grafo gerado" << std::endl;
    // newGraph->generateDot("graph_file");
    // newGraph = newGraph->breadthFirstSearch(5);
    // newGraph->generateDot("125_graph");
    newGraph->floydMarshall(5, 28)->generateDot("test_5_28");
    newGraph->floydMarshall(28, 5)->generateDot("test_28_5");

}