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
#include <vector>
#include <unordered_set>

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
    map<int, Node *> nodesMap;

public:
    //Constructor
    Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
    Graph(bool directed, bool weighted_edge, bool weighted_node);
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
    Graph *breadthFirstSearch(int id);
    Graph *directTransitiveClosure(int id);
    Graph *indirectTransitiveClosure(int id);
    Graph *agmKuskal();
    Graph *agmPrim();
    Graph *floydMarshall(int idSource, int idTarget);
    float dijkstra(int idSource, int idTarget);

    //methods phase1
    float greed();
    float greedRandom();
    float greedRactiveRandom();

    void generateDot(string nome, string layout="dot");

private:
    //Auxiliar methods
    // void auxDirectTransitiveClosure(Node *node, deque<int> &nodesList);
    void auxDirectTransitiveClosure(Node* node, Graph* graph);
    // void auxIndirectTransitiveClosure(Node *node, int &targetId, int startNode, unordered_set<int> &nodesList, vector<int> &visited);
    bool auxIndirectTransitiveClosure(int sourceId, int targetId);
    void pathDistanceDFS(int node, vector<vector<float>>& dist_mat, vector<vector<float>> &path_mat);
};

#endif // GRAPH_H_INCLUDED
