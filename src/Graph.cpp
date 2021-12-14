#include "../include/Graph.h"
#include "../include/Node.h"
#include "../include/Edge.h"
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
    if (!directed)
        target_node->insertEdge(id, weight);
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

Graph *Graph::breadthFirstSearch(int id)
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
    string edgeType;
    map<pair<int, int>, bool> edges_added;

    if (getDirected())
    {
        dot_file << "digraph { " << endl;
        edgeType = "->";
    }
    else
    {
        dot_file << "graph {" << endl;
        edgeType = "--";
    }

    dot_file << "   overlap=false; layout=dot; splines=true;" << endl;
    dot_file << endl
             << "   node [shape=box, style=filled, fillcolor=lightblue]" << endl;
    dot_file << endl
             << "   edge [color=blue, len=20.0, fontsize=15]\n"
             << endl;

    for (Node *node = newGraph->getFirstNode(); node != nullptr; node = node->getNextNode())
    {
        dot_file << "   " << node->getId() << endl;
        for (Edge *edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            if (edges_added[make_pair(edge->getTargetId(), node->getId())] != true)
            {
                edges_added[make_pair(node->getId(), edge->getTargetId())] = true;
                if (getWeightedEdge())
                    dot_file << "   " << node->getId() << edgeType << edge->getTargetId() << " [label=\"" << edge->getWeight() << "\"]" << endl;
                else
                    dot_file << "   " << node->getId() << edgeType << edge->getTargetId() << endl;
            }
        }
    }

    for (auto edge : back_edges)
    {
        if (edges_added[make_pair(get<1>(edge), get<0>(edge))] != true)
        {
            edges_added[make_pair(get<0>(edge), get<1>(edge))] = true;
            if (getWeightedEdge())
                dot_file << "   " << get<0>(edge) << edgeType << get<1>(edge) << " [label=\"" << get<2>(edge) << "\", color=\"red\"]" << endl;
            else
                dot_file << "   " << get<0>(edge) << edgeType << get<1>(edge) << " [color=\"red\"]" << endl;
        }
    }

    dot_file << "}";
    dot_file.close();

    system(string("dot -Tpng ./DOTs/BFS_with_back_edges.dot -o output/BFS_with_back_edges.png").c_str());

    newGraph->order = newGraph->nodesMap.size();

    return newGraph;
}

Graph *Graph::floydMarshall(int idSource, int idTarget)
{
    vector<vector<float>> dist_mat(this->order, vector<float>(this->order));
    vector<vector<float>> path_mat(this->order, vector<float>(this->order, -1.0f));

    for (auto node1 : nodesMap)
        this->pathDistanceDFS(node1.first, dist_mat, path_mat);

    for (int i = 0; i < this->order; i++)
        for (int j = 0; j < this->order; j++)
            if (dist_mat[i][j] == 0 && i != j)
                dist_mat[i][j] = INFINITY;

    // Print dist matrix
    cout << "dist mat:" << endl;
    for (int i = 0; i < dist_mat.size(); i++)
    {
        for (int j = 0; j < dist_mat.size(); j++)
            cout << dist_mat[i][j] << " ";
        cout << endl;
    }
    cout << endl;

    cout << "path mat:" << endl;
    for (int i = 0; i < path_mat.size(); i++)
    {
        for (int j = 0; j < path_mat.size(); j++)
            cout << path_mat[i][j] << " ";
        cout << endl;
    }
    cout << endl;

    for (int k = 0; k < this->order; k++)
    {
        for (int i = 0; i < this->order; i++)
        {
            for (int j = 0; j < this->order; j++)
            {
                if (k != i && k != j && i != j)
                {
                    cout << i << " " << j << " " << k << endl;
                    if (dist_mat[i][k] + dist_mat[k][j] < dist_mat[i][j])
                    {
                        cout << "entrou" << endl;
                        dist_mat[i][j] = dist_mat[i][k] + dist_mat[k][j];
                        cout << path_mat[i][j] << endl;
                        path_mat[i][j] = path_mat[k][j];
                        cout << path_mat[i][j] << endl;
                        cout << "mudou " << i << " " << j << " " << k << endl;
                    }
                }
            }
        }
    }

    // Print dist matrix
    cout << "dist mat:" << endl;
    for (int i = 0; i < dist_mat.size(); i++)
    {
        for (int j = 0; j < dist_mat.size(); j++)
            cout << dist_mat[i][j] << " ";
        cout << endl;
    }
    cout << endl;

    cout << "path mat:" << endl;
    for (int i = 0; i < path_mat.size(); i++)
    {
        for (int j = 0; j < path_mat.size(); j++)
            cout << path_mat[i][j] << " ";
        cout << endl;
    }
    cout << endl;

    Graph *newGraph = new Graph(directed, weighted_edge, weighted_node);

    while (path_mat[idSource][idTarget] != -1)
    {
        newGraph->insertEdge(path_mat[idSource][idTarget], idTarget, dist_mat[path_mat[idSource][idTarget]][idTarget]);
        if (path_mat[idSource][idTarget] == idSource)
        {
            break;
        }
        idTarget = path_mat[idSource][idTarget];
    }

    newGraph->order = newGraph->nodesMap.size();

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

float Graph::dijkstra(int idSource, int idTarget)
{
    unordered_set<int> unvisited;
    map<int, float> dist;
    int currentId;
    float currentDist, tempDist;
    Node *node = this->first_node;
    Edge *edge;

    while (node != nullptr)
    {
        unvisited.insert(node->getId());
        dist.emplace(node->getId(), INFINITY);
        node = node->getNextNode();
    }
    dist[idSource] = 0;

    while (!unvisited.empty())
    {
        currentId = *(unvisited.begin());
        currentDist = dist[currentId];
        for (auto &&id : unvisited)
        {
            if (dist[id] < currentDist)
            {
                currentId = id;
            }
        }
        unvisited.erase(currentId);

        node = this->getNode(currentId);
        edge = node->getFirstEdge();

        while (edge != nullptr)
        {
            tempDist = dist[currentId] + edge->getWeight();
            if (tempDist < dist[edge->getTargetId()])
            {
                dist[edge->getTargetId()] = tempDist;
            }
            edge = edge->getNextEdge();
        }
    }
    return dist[idTarget];
}

// function that prints a topological sorting
void Graph::topologicalSorting()
{
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

    vector<bool> visited(this->order, false);

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

Graph *Graph::agmKuskal()
{
}
Graph *Graph::agmPrim()
{
}

/**
 * @brief Function that generates a .dot file that can be used to generate a image of the graph using Graphviz
 *
 * @param nome
 * @param layout
 */
void Graph::generateDot(string nome, string layout)
{
    ofstream dot_file("DOTs/" + nome + ".dot", ios::out);
    string edgeType;
    map<pair<int, int>, bool> edges_added;
    if (getDirected())
    {
        dot_file << "digraph { " << endl;
        edgeType = "->";
    }
    else
    {
        dot_file << "graph {" << endl;
        edgeType = "--";
    }

    dot_file << "   overlap=false; layout=\"" + layout + "\"; splines=true;" << endl;
    dot_file << endl
             << "   node [shape=box, style=filled, fillcolor=lightblue]" << endl;
    dot_file << endl
             << "   edge [color=blue, len=20.0, fontsize=15]\n"
             << endl;

    for (Node *node = first_node; node != nullptr; node = node->getNextNode())
    {
        dot_file << "   " << node->getId() << endl;
        for (Edge *edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            if (edges_added[make_pair(edge->getTargetId(), node->getId())] != true)
            {
                edges_added[make_pair(node->getId(), edge->getTargetId())] = true;

                if (getWeightedEdge())
                    dot_file << "   " << node->getId() << edgeType << edge->getTargetId() << " [label=\"" << edge->getWeight() << "\"]" << endl;
                else
                    dot_file << "   " << node->getId() << edgeType << edge->getTargetId() << endl;
            }
        }
    }

    dot_file << "}";
    dot_file.close();

    system(string("dot -Tpng ./DOTs/" + nome + ".dot -o output/" + nome + ".png").c_str());
}
