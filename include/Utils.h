#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <iostream>
#include "Graph.h"

using namespace std;

class Utils
{
private:
    /* data */
public:
    Utils(/* args */);
    ~Utils();
    static Graph *createGraph(std::string graph_file_path, bool directed, bool weighted_edge, bool weighted_node);
    static void ChangeGraph(Graph *graph);
    static void CallTopologicalSorting(Graph *graph);
    static void CallKruskal(Graph *graph, string result_dir_path);
    static void CallBFS(Graph *graph, string result_dir_path);
    static void CallDirectTrasitiveClosure(Graph *graph, string result_dir_path);
    static void CallIndirectTrasitiveClosure(Graph *graph, string result_dir_path);
    static void CallDijkstra(Graph *graph, string result_dir_path);
    static void CallFloyd(Graph *graph, string result_dir_path);
};

Utils::Utils(/* args */)
{
}

Utils::~Utils()
{
}

    Graph *Utils::createGraph(std::string graph_file_path, bool directed, bool weighted_edge, bool weighted_node)
    {
        ifstream graph_file;
        graph_file.open(graph_file_path, ios::in);

        int order;
        graph_file >> order;

        Graph *graph = new Graph(order, directed, weighted_edge, weighted_node);

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

    void Utils::ChangeGraph(Graph *graph)
    {
        std::string newGraphPath;
        std::ifstream file_test;
        do
        {
            cout << "Passe o endereço do novo grafo: ";
            cin >> newGraphPath;
            file_test.open(newGraphPath, ios::in);
        } while (!file_test.is_open());
        file_test.close();
        delete graph;

        cout << "Para as perguntas a seguir digite 1 para SIM e 0 para Não" << endl
             << endl;

        bool directed, weighted_edge, weighted_node;
        do
        {
            std::cout << "Direcionado: ";
            cin >> directed;
        } while (directed < 0 || directed > 1);

        do
        {
            std::cout << "Peso nas Arestas: ";
            cin >> weighted_edge;
        } while (weighted_edge < 0 || weighted_edge > 1);

        do
        {
            std::cout << "Peso nos Vértices: ";
            cin >> weighted_node;
        } while (weighted_node < 0 || weighted_node > 1);

        graph = Utils::createGraph(newGraphPath, directed, weighted_edge, weighted_node);
    }

    void Utils::CallTopologicalSorting(Graph *graph)
    {
        graph->topologicalSorting();
    }

    void Utils::CallKruskal(Graph *graph, string result_dir_path)
    {
        Graph *newGraph = graph->agmKruskal();
        newGraph->generateDot("Kruskal_AGM", result_dir_path);
        delete newGraph;
    }

    void Utils::CallBFS(Graph *graph, string result_dir_path)
    {
        int id;
        do
        {
            std::cout << "Digite o ID do vértice de origem: ";
            cin >> id;
        } while (id < 0);

        Graph *newGraph = graph->breadthFirstSearch(id, result_dir_path);
        delete newGraph;
    }

    void Utils::CallDirectTrasitiveClosure(Graph *graph, string result_dir_path)
    {
        int id;
        do
        {
            std::cout << "Digite o ID do vértice que deseja ver o fecho: ";
            cin >> id;
        } while (id < 0);

        Graph *newGraph = graph->directTransitiveClosure(id);
        newGraph->generateDot("Direct_Trasitive_Closure", result_dir_path);
        delete newGraph;
    }

    void Utils::CallIndirectTrasitiveClosure(Graph *graph, string result_dir_path)
    {
        int id;
        do
        {
            std::cout << "Digite o ID do vértice que deseja ver o fecho: ";
            cin >> id;
        } while (id < 0);

        Graph *newGraph = graph->indirectTransitiveClosure(id);
        newGraph->generateDot("Indirect_Trasitive_Closure", result_dir_path);
        delete newGraph;
    }

    void Utils::CallDijkstra(Graph *graph, string result_dir_path)
    {
        int idSource, idTarget;
        do
        {
            std::cout << "Digite o ID do vértice de origem: ";
            cin >> idSource;
        } while (idSource < 0);

        do
        {
            std::cout << "Digite o ID do vértice de destino: ";
            cin >> idTarget;
        } while (idTarget < 0);

        //cout << "Distancia: " << graph->dijkstra(idSource, idTarget) << endl;
        // TODO Descomentar as linhas de baixo depois que o dijkstra começar a retornar um graph do caminho
         Graph* newGraph = graph->dijkstra(idSource, idTarget);
         newGraph->generateDot("Caminho_Minimo_Dijkstra", result_dir_path);
         delete newGraph;
    }

    void Utils::CallFloyd(Graph *graph, string result_dir_path)
    {
        int idSource, idTarget;
        do
        {
            std::cout << "Digite o ID do vértice de origem: ";
            cin >> idSource;
        } while (idSource < 0);

        do
        {
            std::cout << "Digite o ID do vértice de destino: ";
            cin >> idTarget;
        } while (idTarget < 0);

        Graph *newGraph = graph->floydMarshall(idSource, idTarget);
        newGraph->generateDot("Caminho_Minimo_Floyd-Marshall", result_dir_path);
        delete newGraph;
    }
#endif