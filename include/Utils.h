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
    static Graph *createGraphPart2(std::string graph_file_path, bool directed, bool weighted_edge, bool weighted_node);
    static void ChangeGraph(Graph *graph);
    static void CallTopologicalSorting(Graph *graph);
    static void CallKruskal(Graph *graph, string result_dir_path);
    static void CallBFS(Graph *graph, string result_dir_path);
    static void CallDirectTrasitiveClosure(Graph *graph, string result_dir_path);
    static void CallIndirectTrasitiveClosure(Graph *graph, string result_dir_path);
    static void CallDijkstra(Graph *graph, string result_dir_path);
    static void CallFloyd(Graph *graph, string result_dir_path);
};

#endif