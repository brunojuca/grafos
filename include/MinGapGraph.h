#ifndef MINGAPGRAPH_H
#define MINGAPGRAPH_H
#include "Graph.h"

class MinGapGraph : public Graph
{
    using Graph::Graph;

private:
public:
    int minNodeWeight;
    int maxNodeWeight;
    list<pair<int, int>> candidates;

    void insertEdge(int id, int target_id, int sourceWeight, int targetWeight, float weight = 1);

    // bool compare(tuple<int, int, int>& a, tuple<int, int, int>& b)
    // {
    //     return
    // }
};

#endif