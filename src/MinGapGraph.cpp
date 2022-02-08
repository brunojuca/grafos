#include "MinGapGraph.h"

void MinGapGraph::insertEdge(int id, int target_id, int sourceWeight, int targetWeight, float weight)
{
    Node *source_node = this->getNode(id);
    Node *target_node = this->getNode(target_id);

    if (this->order == 0)
    {
        this->minNodeWeight = sourceWeight;
        this->maxNodeWeight = sourceWeight;
    }

    if (sourceWeight > this->maxNodeWeight)
    {
        this->maxNodeWeight = sourceWeight;
    }
    else if (sourceWeight < this->minNodeWeight)
    {
        this->minNodeWeight = sourceWeight;
    }

    if (targetWeight > this->maxNodeWeight)
    {
        this->maxNodeWeight = targetWeight;
    }
    else if (targetWeight < this->minNodeWeight)
    {
        this->minNodeWeight = targetWeight;
    }

    // If Graph does not have source node
    if (source_node == nullptr) {
        source_node = this->insertNode(id, sourceWeight);
        this->order++;
    }
    // If Graph does not have target node
    if (target_node == nullptr){
        target_node = this->insertNode(target_id, targetWeight);
        this->order++;
    }

    source_node->insertEdge(target_id, weight);
    source_node->incrementOutDegree();
    target_node->incrementInDegree();
    if (!this->directed)
    {
        target_node->incrementOutDegree();
        source_node->incrementInDegree();
        target_node->insertEdge(id, weight);
    }
    this->number_edges++;
}