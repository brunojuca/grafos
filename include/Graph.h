/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>
#include <map>

using namespace std;

class Graph
{

    //Atributes
private:
    int order;
    int number_edges;
    bool directed;
    bool weighted_edge;
    bool weighted_node;
    Node *first_node;
    Node *last_node;
    map<int, Node*> nodesMap;

public:
    //Constructor
    Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
    //Destructor
    ~Graph();
    //Getters
    int getOrder();
    int getNumberEdges();
    bool getDirected();
    bool getWeightedEdge();
    bool getWeightedNode();
    Node *getFirstNode();
    Node *getLastNode();
    //Other methods
    Node *insertNode(int id, float weight = 1);
    void insertEdge(int id, int target_id, float weight = 1);
    void removeNode(int id);
    bool searchNode(int id);
    Node *getNode(int id);

    //methods phase1
    void topologicalSorting();
    void breadthFirstSearch(ofstream &output_file);
    Graph *transitiveClosure(int id);
    Graph *agmKuskal();
    Graph *agmPrim();
    float floydMarshall(int idSource, int idTarget);
    float dijkstra(int idSource, int idTarget);

    //methods phase1
    float greed();
    float greedRandom();
    float greedRactiveRandom();

    void generateDot(string nome);
private:
    //Auxiliar methods
    void auxTransitiveClosure(Node *node, deque<int>& nodesList);


};

#endif // GRAPH_H_INCLUDED
