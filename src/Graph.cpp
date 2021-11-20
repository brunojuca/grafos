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
#include <map>
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
//Function that verifies if the graph is directed
bool Graph::getDirected()
{

    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

//Function that verifies if the graph is weighted at the nodes
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

//Function that prints a set of edges belongs breadth tree

void Graph::breadthFirstSearch(ofstream &output_file)
{
}

float Graph::floydMarshall(int idSource, int idTarget)
{
}

float Graph::dijkstra(int idSource, int idTarget)
{
}

//function that prints a topological sorting
void Graph::topologicalSorting()
{
}

Graph *Graph::transitiveClosure(int id)
{
    if(this->getFirstNode() == nullptr || this->getNode(id) == nullptr)
        return nullptr;
    
    Node *node = this->getNode(id);
    deque<int> nodesList;
    auxTransitiveClosure(node, nodesList);
    Graph *newGraph = new Graph(nodesList.size(), this->directed, this->weighted_edge, this->weighted_node);
    for (int nodeId : nodesList)
    {
        Node *oldNode = this->getNode(nodeId);
        Node *newNode;

        if (!newGraph->searchNode(nodeId))
            newNode = newGraph->insertNode(nodeId, node->getWeight());
        else
            newNode = newGraph->getNode(nodeId);
        for (Edge *edge = oldNode->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
        {
            if (!newGraph->searchNode(edge->getTargetId()))
                newGraph->insertNode(edge->getTargetId(), this->getNode(edge->getTargetId())->getWeight());
            newNode->insertEdge(edge->getTargetId(), edge->getWeight());
            newGraph->number_edges++;
        }
    }

    return newGraph;
}

void Graph::auxTransitiveClosure(Node *node, deque<int> &nodesList)
{
    for (Edge *edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
    {
        if (find(nodesList.begin(), nodesList.end(), edge->getTargetId()) == nodesList.end())
        {
            nodesList.push_back(edge->getTargetId());
            auxTransitiveClosure(getNode(edge->getTargetId()), nodesList);
        }
    }
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
 */
void Graph::generateDot(string nome)
{
	ofstream dot_file("DOTs/" + nome + ".dot", ios::out);
	string edgeType;
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

	dot_file << "   overlap=false; layout=neato; splines=true;" << endl;
	dot_file << endl
		   << "   node [shape=box, style=filled, fillcolor=lightblue]" << endl;
	dot_file << endl
		   << "   edge [color=blue, len=20.0, fontsize=20, fontcolor=darkblue]\n"
		   << endl;

	for (Node *node = first_node; node != nullptr; node = node->getNextNode())
	{
        dot_file << "   " << node->getId() << endl;
		for (Edge *edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
		{
			if (getWeightedEdge())
				dot_file << "   " << node->getId() << edgeType << edge->getTargetId() << " [label=\"" << edge->getWeight() << "\"]" << endl;
			else
				dot_file << "   " << node->getId() << edgeType << edge->getTargetId() << endl;
		}
	}

	dot_file << "}";
	dot_file.close();

    system(string("dot -Tpng ./DOTs/" + nome + ".dot -o output/" + nome + ".png").c_str());
}
