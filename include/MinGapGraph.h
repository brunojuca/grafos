#ifndef MINGAPGRAPH_H
#define MINGAPGRAPH_H
#include "Graph.h"

class MinGapGraph : public Graph
{
     using Graph::Graph;

private:
    /* data */
public:
    // MinGapGraph()
    // {
    //     Graph();
    // }

    int minNodeWeight;
    int maxNodeWeight;

    void insertEdge(int id, int target_id, int sourceWeight, int targetWeight, float weight = 1);
};

#endif