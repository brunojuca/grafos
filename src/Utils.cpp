#include "Utils.h"
#include <algorithm>
#include "set"

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

Graph *Utils::createGraphPart2(std::string graph_file_path, bool directed, bool weighted_edge, bool weighted_node)
{
    ifstream graph_file;
    string word, weight;
    graph_file.open(graph_file_path, ios::in);

    int order;

    do
        graph_file >> word;
    while (word != "p");
    graph_file >> word >> order;

    Graph *graph = new Graph(directed, weighted_edge, weighted_node);

    do
        graph_file >> word;
    while (word != "set");
    graph_file >> word >> word >> word;

    while (word != ";")
    {
        graph->insertNode(stoi(word));
        graph_file >> word;
    }

    do
        graph_file >> word;
    while (word != "param");
    graph_file >> word >> word >> word;

    while (word != ";")
    {
        graph_file >> weight;
        graph->getNode(stoi(word))->setWeight(stoi(weight));
        graph_file >> word;
    }

    do
        graph_file >> word;
    while (word != "set");
    graph_file >> word >> word >> word;

    while (word != ";")
    {
        int idNodeSource = stoi(word.substr(1, word.find(",") - 1));
        int idNodeTarget = stoi(word.substr(word.find(",") + 1, word.size() - word.find(",") - 2));

        graph->insertEdge(idNodeSource, idNodeTarget);

        graph_file >> word;
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

    // cout << "Distancia: " << graph->dijkstra(idSource, idTarget) << endl;
    //  TODO Descomentar as linhas de baixo depois que o dijkstra começar a retornar um graph do caminho
    Graph *newGraph = graph->dijkstra(idSource, idTarget);
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

vector<MinGapGraph> Utils::greed(Graph *graph, int p)
{

    vector<pair<int, int>> edges;
    set<int> insertedNodes;

    for (Node *node = graph->getFirstNode(); node != nullptr; node = node->getNextNode())
        for (Edge *edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
            if (node->getId() < edge->getTargetId())
                edges.push_back(make_pair(node->getId(), edge->getTargetId()));

    sort(edges.begin(), edges.end(), [graph](pair<int, int> a, pair<int, int> b)
         { return abs(graph->getNode(a.first)->getWeight() - graph->getNode(a.second)->getWeight()) > abs(graph->getNode(b.first)->getWeight() - graph->getNode(b.second)->getWeight()); });

    // print edges
    // for (auto edge : edges)
    // {
    //     cout << edge.first << " " << edge.second << " " << abs(graph->getNode(edge.first)->getWeight() - graph->getNode(edge.second)->getWeight()) << endl;
    // }

    cout << "\nNumero de edges: " << edges.size() << endl;

    vector<MinGapGraph> partitions(p);

    int insertedEdges = 0;
    while (insertedEdges < p)
    {
        if (insertedNodes.find(edges.back().first) == insertedNodes.end() && insertedNodes.find(edges.back().second) == insertedNodes.end())
        {
            partitions[insertedEdges].insertEdge(edges.back().first, edges.back().second, graph->getNode(edges.back().first)->getWeight(), graph->getNode(edges.back().second)->getWeight());
            insertedNodes.insert(edges.back().first);
            insertedNodes.insert(edges.back().second);
            insertedEdges++;
        }
        edges.pop_back();
    }

    // print partitions
    // for (auto &&part : partitions)
    // {
    //     cout << "part" << endl;
    //     for (Node* n = part.getFirstNode(); n != nullptr; n = n->getNextNode())
    //     {
    //         cout << n->getId() << " ";
    //     } cout << part.maxNodeWeight << " " << part.minNodeWeight << endl;
        
    // }
    
    vector<pair<int,int>> candidates;

    for (auto &&part : partitions)
    {
        for (Node *n = part.getFirstNode(); n != nullptr; n = n->getNextNode())
        {
            Node *graphNode = graph->getNode(n->getId());
            for (Edge *e = graphNode->getFirstEdge(); e != nullptr; e = e->getNextEdge())
            {
                if (insertedNodes.find(e->getTargetId()) == insertedNodes.end())
                {

                     cout << n->getId() << " " << e->getTargetId() << endl;
                    candidates.push_back(make_pair(n->getId(), e->getTargetId()));
                    /* code */
                }
            }
        }
        cout << endl;
    }

    // sort(candidates.begin(), candidates.end(), [&partitions](pair<int, int> a, pair<int, int> b)
    //     { 
    //         int diffA = 0;
    //         int diffB = 0;

    //         for (auto &&part : partitions)
    //         {
    //             for (Node *n = part.getFirstNode(); n != nullptr; n = n->getNextNode())
    //             {
    //                 cout << part.minNodeWeight << " ";
    //             }
    //         }

    //         return true; 
    //     });

    return partitions;
}