#include "../include/Graph.h"
#include "../include/Node.h"
#include "../include/Edge.h"
#include "../include/MinGapGraph.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <deque>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
 **************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{
    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;

    for (int i = 1; i < order; i++)
        this->insertNode(i);
}

Graph::Graph(bool directed, bool weighted_edge, bool weighted_node)
{
    this->order = 0;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

Graph::Graph()
{
    this->order = 0;
    this->directed = 0;
    this->weighted_edge = 0;
    this->weighted_node = 1;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{

    return this->order;
}
int Graph::getNumberEdges()
{

    return this->number_edges;
}
// Function that verifies if the graph is directed
bool Graph::getDirected()
{

    return this->directed;
}
// Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

// Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{

    return this->weighted_node;
}

Node *Graph::getFirstNode()
{

    return this->first_node;
}

Node *Graph::getLastNode()
{

    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/

/**
 * @brief Insert a new node into Graph and returns a pointer to the created node
 *
 * @param id New node id
 * @param weight New node weight
 * @return Node*
 */
Node *Graph::insertNode(int id, float weight)
{
    Node *newNode = new Node(id, weight);

    if (this->getFirstNode() == nullptr)
        this->first_node = this->last_node = newNode; // TODO Decide if we are gonna make setters
    else
    {
        this->getLastNode()->setNextNode(newNode);
        this->last_node = newNode;
    }

    this->nodesMap[id] = newNode;

    return newNode;
}

void Graph::insertEdge(int id, int target_id, float weight)
{
    Node *source_node = this->getNode(id);
    Node *target_node = this->getNode(target_id);

    // If Graph does not have source node
    if (source_node == nullptr)
        source_node = this->insertNode(id);
    // If Graph does not have target node
    if (target_node == nullptr)
        target_node = this->insertNode(target_id);

    source_node->insertEdge(target_id, weight);
    source_node->incrementOutDegree();
    target_node->incrementInDegree();
    if (!directed)
    {
        target_node->incrementOutDegree();
        source_node->incrementInDegree();
        target_node->insertEdge(id, weight);
    }
    this->number_edges++;
}

void Graph::removeNode(int id)
{
}

/**
 * @brief Search node by Id. Returns true if node exists already.
 *
 * @param id
 * @return true
 * @return false
 */
bool Graph::searchNode(int id)
{
    return nodesMap[id] != nullptr;
}

/**
 * @brief Search and get node by Id. Returns a pointer to the node or nullptr if not found.
 *
 * @param id
 * @return Node*
 */
Node *Graph::getNode(int id)
{
    return nodesMap[id];
}

// Function that prints a set of edges belongs breadth tree

Graph *Graph::breadthFirstSearch(int id, string result_dir_path)
{
    vector<bool> visited(this->order, false);
    vector<bool> completed(this->order, false);

    queue<int> q;

    q.push(id);
    visited[id] = true;

    Graph *newGraph = new Graph(this->directed, this->weighted_edge, this->weighted_node);
    newGraph->insertNode(id, this->getNode(id)->getWeight());

    vector<tuple<int, int, int>> back_edges;

    while (!q.empty())
    {
        id = q.front();
        q.pop();
        completed[id] = true;
        for (Edge *edge = this->getNode(id)->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            if (!visited[edge->getTargetId()])
            {
                newGraph->insertNode(edge->getTargetId(), this->getNode(edge->getTargetId())->getWeight());
                newGraph->insertEdge(id, edge->getTargetId(), edge->getWeight());
                visited[edge->getTargetId()] = true;
                q.push(edge->getTargetId());
            }
            else
            {
                if (!newGraph->getDirected())
                {
                    if (!completed[edge->getTargetId()])
                        back_edges.push_back(make_tuple(id, edge->getTargetId(), edge->getWeight()));
                }
                else
                    back_edges.push_back(make_tuple(id, edge->getTargetId(), edge->getWeight()));
            }
        }
    }

    ofstream dot_file("DOTs/BFS_with_back_edges.dot", ios::out);
    ofstream outFile(result_dir_path, ios::app);
    string edgeType;
    map<pair<int, int>, bool> edges_added;

    outFile << "BFS_with_back_edges" << endl;

    if (getDirected())
    {
        dot_file << "digraph { " << endl;
        outFile << "digraph { " << endl;
        edgeType = "->";
    }
    else
    {
        dot_file << "graph {" << endl;
        outFile << "graph {" << endl;
        edgeType = "--";
    }

    dot_file << "   overlap=false; layout=dot; splines=true;" << endl;
    outFile << "   overlap=false; layout=dot; splines=true;" << endl;
    dot_file << endl
             << "   node [shape=box, style=filled, fillcolor=lightblue]" << endl;
    outFile << endl
             << "   node [shape=box, style=filled, fillcolor=lightblue]" << endl;
    dot_file << endl
             << "   edge [color=blue, len=20.0, fontsize=15]\n"
             << endl;
    outFile << endl
             << "   edge [color=blue, len=20.0, fontsize=15]\n"
             << endl;

    for (Node *node = newGraph->getFirstNode(); node != nullptr; node = node->getNextNode())
    {
        dot_file << "   " << node->getId() << endl;
        outFile << "   " << node->getId() << endl;
        for (Edge *edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            if (edges_added[make_pair(edge->getTargetId(), node->getId())] != true)
            {
                edges_added[make_pair(node->getId(), edge->getTargetId())] = true;
                if (getWeightedEdge()) {
                    dot_file << "   " << node->getId() << edgeType << edge->getTargetId() << " [label=\"" << edge->getWeight() << "\"]" << endl;
                    outFile << "   " << node->getId() << edgeType << edge->getTargetId() << " [label=\"" << edge->getWeight() << "\"]" << endl;
                }
                else {
                    dot_file << "   " << node->getId() << edgeType << edge->getTargetId() << endl;
                    outFile << "   " << node->getId() << edgeType << edge->getTargetId() << endl;
                }
            }
        }
    }

    for (auto edge : back_edges)
    {
        if (edges_added[make_pair(get<1>(edge), get<0>(edge))] != true)
        {
            edges_added[make_pair(get<0>(edge), get<1>(edge))] = true;
            if (getWeightedEdge()) {
                dot_file << "   " << get<0>(edge) << edgeType << get<1>(edge) << " [label=\"" << get<2>(edge) << "\", color=\"red\"]" << endl;
                outFile << "   " << get<0>(edge) << edgeType << get<1>(edge) << " [label=\"" << get<2>(edge) << "\", color=\"red\"]" << endl;
            }
                
            else {
                dot_file << "   " << get<0>(edge) << edgeType << get<1>(edge) << " [color=\"red\"]" << endl;
                outFile << "   " << get<0>(edge) << edgeType << get<1>(edge) << " [color=\"red\"]" << endl;
            }
        }
    }

    dot_file << "}";
    outFile << "}";
    outFile << endl << endl;
    dot_file.close();
    outFile.close();

    //system(string("dot -Tpng ./DOTs/BFS_with_back_edges.dot -o output/BFS_with_back_edges.png").c_str());

    newGraph->order = newGraph->nodesMap.size();

    return newGraph;
}

Graph *Graph::floydMarshall(int idSource, int idTarget)
{
    if (idSource == idTarget)
    {
        cout << "Origem e destino iguais" << endl;
    }
    vector<vector<float>> dist_mat(this->order, vector<float>(this->order));
    vector<vector<float>> path_mat(this->order, vector<float>(this->order, -1.0f));

    for (auto node1 : nodesMap)
        this->pathDistanceDFS(node1.first, dist_mat, path_mat);

    for (int i = 0; i < this->order; i++)
        for (int j = 0; j < this->order; j++)
            if (dist_mat[i][j] == 0 && i != j)
                dist_mat[i][j] = INFINITY;

    for (int k = 0; k < this->order; k++)
    {
        for (int i = 0; i < this->order; i++)
        {
            for (int j = 0; j < this->order; j++)
            {
                if (k != i && k != j && i != j)
                {
                    if (dist_mat[i][k] + dist_mat[k][j] < dist_mat[i][j])
                    {
                        dist_mat[i][j] = dist_mat[i][k] + dist_mat[k][j];
                        path_mat[i][j] = path_mat[k][j];
                    }
                }
            }
        }
    }

    Graph *newGraph = new Graph(directed, weighted_edge, weighted_node);
    string menor_caminho = "";
    while (path_mat[idSource][idTarget] != -1)
    {
        newGraph->insertEdge(path_mat[idSource][idTarget], idTarget, dist_mat[path_mat[idSource][idTarget]][idTarget]);
        menor_caminho = " -> " + to_string(idTarget) + menor_caminho;
        if (path_mat[idSource][idTarget] == idSource)
            break;

        idTarget = path_mat[idSource][idTarget];
    }

    menor_caminho = to_string(idSource) + menor_caminho;

    newGraph->order = newGraph->nodesMap.size();

    cout << "Menor Caminho usando o algoritmo de Floyd-Marshall:" << endl;
    cout << menor_caminho << endl;

    return newGraph;
}

void Graph::pathDistanceDFS(int node, vector<vector<float>> &dist_mat, vector<vector<float>> &path_mat)
{
    vector<bool> visited(this->order, false);

    queue<int> q;
    queue<float> distance;
    distance.push(0.0f);
    int id = node;
    q.push(id);
    visited[id] = true;
    while (!q.empty())
    {
        id = q.front();
        q.pop();
        for (Edge *edge = this->getNode(id)->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            if (!visited[edge->getTargetId()])
            {
                dist_mat[node][edge->getTargetId()] = distance.front() + edge->getWeight();
                path_mat[node][edge->getTargetId()] = id;
                visited[edge->getTargetId()] = true;
                q.push(edge->getTargetId());
                distance.push(dist_mat[node][edge->getTargetId()]);
            }
        }
        distance.pop();
    }
}

/**
 * @brief
 *
 * @param idSource
 * @param idTarget
 * @return pair<float, int> first is the distance and second is the
 */

Graph *Graph::dijkstra(int idSource, int idTarget)
{
    unordered_set<int> unvisited;
    map<int, pair<float, int>> dist;
    int currentId;
    pair<float, int> currentDist, tempDist;
    Node *node = this->first_node;
    Edge *edge;
    vector<int> resultPath;

    while (node != nullptr)
    {
        unvisited.insert(node->getId());
        dist.emplace(node->getId(), make_pair(INFINITY, -1));
        node = node->getNextNode();
    }
    dist[idSource] = make_pair(0, idSource);

    while (!unvisited.empty())
    {
        currentId = *(unvisited.begin());
        currentDist = dist[currentId];
        for (auto &&id : unvisited)
        {
            if (dist[id].first < currentDist.first)
            {
                currentId = id;
            }
        }
        unvisited.erase(currentId);

        node = this->getNode(currentId);
        edge = node->getFirstEdge();

        while (edge != nullptr)
        {
            tempDist = make_pair(dist[currentId].first + edge->getWeight(), currentId);
            if (tempDist.first < dist[edge->getTargetId()].first)
            {
                dist[edge->getTargetId()] = tempDist;
            }
            edge = edge->getNextEdge();
        }
    }

    int pathId = idTarget;
    resultPath.insert(resultPath.begin(), idTarget);
    Graph *newGraph = new Graph(directed, weighted_edge, weighted_node);

    if (dist[idTarget].second == -1)
    {
        cout << "Nao ha caminho entre os vertices escolhidos" << endl;
        return newGraph;
    }

    while (pathId != idSource)
    {
        cout << pathId << endl;
        newGraph->insertEdge(dist[pathId].second, pathId, dist[pathId].first - dist[dist[pathId].second].first);
        resultPath.insert(resultPath.begin(), dist[pathId].second);
        pathId = dist[pathId].second;
    }

    cout << "Menor Caminho usando o algoritmo de Dijkstra (Tamanho: " << dist[idTarget].first << "): " << endl;
    for (auto &&id : resultPath)
    {
        if (id == idSource)
            cout << id;
        else
            cout << " -> " << id;
    }
    cout << endl;

    return newGraph;
}

bool TSCompareFunction(pair<int, int> a, pair<int, int> b);

bool TSCompareFunction(pair<int, int> a, pair<int, int> b)
{
    return a.second < b.second;
}
// function that prints a topological sorting
void Graph::topologicalSorting()
{
    if (this->isCyclic() || !this->directed)
    {
        cout << endl
             << "Grafo é cíclico" << endl
             << endl;
        return;
    }

    vector<int> inDegrees(this->order);

    for (auto nodePair : this->nodesMap)
        inDegrees[nodePair.first] = nodePair.second->getInDegree();

    int i;
    int visitedCount = 0;
    while (visitedCount != this->order)
    {
        for (i = 0; i < inDegrees.size(); i++)
        {
            if (inDegrees[i] == 0)
                break;
        }
        cout << i;
        inDegrees[i]--;
        visitedCount++;

        Node *node = this->getNode(i);

        for (Edge *edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            cout << edge->getTargetId();
            inDegrees[edge->getTargetId()]--;
        }

        if (visitedCount != this->order)
            cout << " -> ";
    }
    cout << endl;
}

/**
 * @brief Function that return a vertex-induced subgraphby the direct transitive closure of the given vertex
 *
 * @param id
 * @return Graph*
 */
Graph *Graph::directTransitiveClosure(int id)
{
    Graph *graph = new Graph(this->directed, this->weighted_edge, this->weighted_node);
    graph->insertNode(id, this->getNode(id)->getWeight());

    if (this->nodesMap.size() < 1)
    {
        graph->order = graph->nodesMap.size();
        return graph;
    }

    vector<bool>
        visited(this->order, false);

    queue<int> q;
    q.push(id);
    visited[id] = true;
    while (!q.empty())
    {
        id = q.front();
        q.pop();
        for (Edge *edge = this->getNode(id)->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            if (!visited[edge->getTargetId()])
            {
                visited[edge->getTargetId()] = true;
                q.push(edge->getTargetId());
                graph->insertEdge(id, edge->getTargetId(), edge->getWeight());
            }
        }
    }

    for (int i = 0; i < this->order; i++)
        if (visited[i])
            for (Edge *edge = this->getNode(i)->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
                if (visited[edge->getTargetId()])
                    graph->insertEdge(i, edge->getTargetId(), edge->getWeight());

    graph->order = graph->nodesMap.size();

    return graph;
}

Graph *Graph::indirectTransitiveClosure(int id)
{
    Graph *graph = new Graph(this->directed, this->weighted_edge, this->weighted_node);
    graph->insertNode(id, this->getNode(id)->getWeight());

    vector<bool> nodes_contained(this->order, false);

    for (pair<int, Node *> nodePair : this->nodesMap)
        nodes_contained[nodePair.first] = this->auxIndirectTransitiveClosure(nodePair.first, id);

    nodes_contained[id] = true;

    for (int i = 0; i < this->order; i++)
        if (nodes_contained[i] && i != id)
            for (Edge *edge = this->getNode(i)->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
                if (nodes_contained[edge->getTargetId()])
                    graph->insertEdge(i, edge->getTargetId(), edge->getWeight());

    graph->order = graph->nodesMap.size();

    return graph;
}

bool Graph::auxIndirectTransitiveClosure(int sourceId, int targetId)
{
    vector<bool> visited(this->order, false);

    queue<int> q;
    int id = sourceId;
    q.push(id);
    visited[id] = true;
    while (!q.empty())
    {
        id = q.front();
        q.pop();
        for (Edge *edge = this->getNode(id)->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            if (!visited[edge->getTargetId()])
            {
                if (edge->getTargetId() == targetId)
                    return true;
                visited[edge->getTargetId()] = true;
                q.push(edge->getTargetId());
            }
        }
    }

    return false;
}

bool kruskalCompFunction(tuple<int, int, float> edge1, tuple<int, int, float> edge2);

bool kruskalCompFunction(tuple<int, int, float> edge1, tuple<int, int, float> edge2)
{
    return get<2>(edge1) < get<2>(edge2);
}

bool Graph::findBy(int origin_node, int end_node)
{
    cout << "teste findby" << endl;
    Graph *graph = this->directTransitiveClosure(origin_node);
    cout << "teste findby" << endl;
    if (graph->getNode(end_node) != nullptr)
    {
        delete graph;
        return true;
    }
    else
    {
        delete graph;
        return false;
    }
}

Graph *Graph::agmKruskal()
{
    Graph *newGraph = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);

    cout << " teste" << endl;

    vector<tuple<int, int, float>> edges;
    for (auto nodePair : nodesMap)
        for (Edge *edge = this->getNode(nodePair.first)->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
            if (find(edges.begin(), edges.end(), make_tuple(edge->getTargetId(), nodePair.first, edge->getWeight())) == edges.end())
                edges.push_back(make_tuple(nodePair.first, edge->getTargetId(), edge->getWeight()));

    cout << " teste" << endl;

    sort(edges.begin(), edges.end(), kruskalCompFunction);
    int alone_nodes = this->order;
    int contador = 0;
    cout << " teste" << endl;
    while (newGraph->number_edges != this->order - 1 && !edges.empty())
    {
        cout << "teste" << endl;
        auto edge = edges.front();
        edges.erase(edges.begin());
        cout << get<0>(edge) << endl;
        if (!newGraph->findBy(get<0>(edge), get<1>(edge)))
            newGraph->insertEdge(get<0>(edge), get<1>(edge), get<2>(edge));
    }

    return newGraph;
}
void Graph::agmPrim()
{
}

bool Graph::isCyclic()
{
    for (Node *node = this->getFirstNode(); node != nullptr; node = node->getNextNode())
    {
        vector<bool> visited(this->order, false);

        queue<int> q;
        int id = node->getId();
        q.push(id);
        visited[id] = true;
        while (!q.empty())
        {
            id = q.front();
            q.pop();
            for (Edge *edge = this->getNode(id)->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
            {
                if (edge->getTargetId() == node->getId())
                {
                    cout << id << " " << edge->getTargetId() << endl;
                    return true;
                }
                if (!visited[edge->getTargetId()])
                {
                    visited[edge->getTargetId()] = true;
                    q.push(edge->getTargetId());
                }
            }
        }
    }
    return false;
}
/**
 * @brief Function that generates a .dot file that can be used to generate a image of the graph using Graphviz
 *
 * @param nome
 * @param layout
 */
void Graph::generateDot(string nome, string outFileStr, string layout)
{
    ofstream dot_file("DOTs/" + nome + ".dot", ios::out);
    ofstream outFile(outFileStr, ios::app);
    string edgeType;
    map<pair<int, int>, bool> edges_added;

    outFile << nome << endl << endl;

    if (getDirected())
    {
        dot_file << "digraph { " << endl;
        outFile << "digraph { " << endl;
        edgeType = "->";
    }
    else
    {
        dot_file << "graph {" << endl;
        outFile << "graph {" << endl;
        edgeType = "--";
    }

    dot_file << "   overlap=false; layout=\"" + layout + "\"; splines=true;" << endl;
    outFile << "   overlap=false; layout=\"" + layout + "\"; splines=true;" << endl;
    dot_file << endl
             << "   node [shape=box, style=filled, fillcolor=lightblue]" << endl;
    outFile << endl
             << "   node [shape=box, style=filled, fillcolor=lightblue]" << endl;
    dot_file << endl
             << "   edge [color=blue, len=20.0, fontsize=15]\n"
             << endl;
    outFile << endl
             << "   edge [color=blue, len=20.0, fontsize=15]\n"
             << endl;

    for (Node *node = first_node; node != nullptr; node = node->getNextNode())
    {
        dot_file << "   " << node->getId() << endl;
        outFile << "   " << node->getId() << endl;
        for (Edge *edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            if (edges_added[make_pair(edge->getTargetId(), node->getId())] != true)
            {
                edges_added[make_pair(node->getId(), edge->getTargetId())] = true;

                if (getWeightedEdge()) {
                    dot_file << "   " << node->getId() << edgeType << edge->getTargetId() << " [label=\"" << edge->getWeight() << "\"]" << endl;
                    outFile << "   " << node->getId() << edgeType << edge->getTargetId() << " [label=\"" << edge->getWeight() << "\"]" << endl;
                }
                else {
                    dot_file << "   " << node->getId() << edgeType << edge->getTargetId() << endl;
                    outFile << "   " << node->getId() << edgeType << edge->getTargetId() << endl;
                }
            }
        }
    }

    dot_file << "}";
    outFile << "}";
    outFile << endl << endl;
    dot_file.close();
    outFile.close();

    //system(string("dot -Tpng ./DOTs/" + nome + ".dot -o output/" + nome + ".png").c_str());
}
