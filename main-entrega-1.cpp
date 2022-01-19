#include <Graph.h>
#include <Edge.h>
#include <Node.h>
#include <iostream>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Utils.h"

// int main(char* graph_file_path, bool directed, bool weighted_edge, bool weighted_node)
int mainEntrega1(int argsc, char *args[])
{
    std::string result_dir_path(args[2]);
    //std::string mkdir_command = "mkdir \"" + result_dir_path + "\"";
    //system(mkdir_command.c_str()); // TODO Arrumar isso aqui

    Graph *graph = Utils::createGraph(args[1], stoi(args[3]), stoi(args[4]), stoi(args[5]));

    //graph->generateDot("Grafo_basico", result_dir_path);

    int option;

    std::cout << std::endl
              << "---------------- Bem Vindo ----------------" << std::endl
              << std::endl;
    do
    {
        std::cout << "O que deseja fazer com o grafo passado:" << endl
                  << endl;

        std::cout << "(1) Fecho Transitivo Direto" << std::endl;
        std::cout << "(2) Fecho Transitivo Indireto" << std::endl;
        std::cout << "(3) Caminho Mínimo usando Algoritmo de Dijkstra" << std::endl;
        std::cout << "(4) Caminho Mínimo usando Algoritmo de Floyd-Marshall" << std::endl;
        std::cout << "(5) Prim" << std::endl;    // TODO
        std::cout << "(6) Kruskal" << std::endl; // TODO
        std::cout << "(7) Árvore gerada por Caminhamento em Largura" << std::endl;
        std::cout << "(8) Ordenação Topológica" << std::endl
                  << std::endl;

        std::cout << "(9) Trocar" << std::endl;
        std::cout << "(0) Sair" << std::endl;
        std::cout << endl
                  << "-------------------------------------------" << std::endl
                  << std::endl;

        std::cout << "Opção: ";
        cin >> option;
        std::cout << std::endl;
        switch (option)
        {
        case 1:
            Utils::CallDirectTrasitiveClosure(graph, result_dir_path);
            break;

        case 2:
            Utils::CallIndirectTrasitiveClosure(graph, result_dir_path);
            break;

        case 3:
            Utils::CallDijkstra(graph, result_dir_path);
            break;

        case 4:
            Utils::CallFloyd(graph, result_dir_path);
            break;
        case 5:
            cout << "Nao implementado" << endl;
            break;

        case 6:
            cout << "Implementacao iniciada, mas nao terminada" << endl;
            //Utils::CallKruskal(graph, result_dir_path);
            break;

        case 7:
            Utils::CallBFS(graph, result_dir_path);
            break;

        case 8:
            Utils::CallTopologicalSorting(graph);
            break;

        case 9:
            Utils::ChangeGraph(graph);
            break;

        default:
            break;
        }

        cout << "-------------------------------------------" << endl
             << endl;
    } while (option != 0);

    cout << "é ciclico? " << graph->isCyclic() << endl;

    delete graph;

    Graph grafo(8, true, false, false);

    grafo.insertNode(4);
    grafo.insertNode(1);
    grafo.insertNode(2);
    grafo.insertNode(3);
    grafo.insertNode(5);
    grafo.insertNode(0);
    grafo.insertNode(6);
    grafo.insertNode(7);

    grafo.insertEdge(1, 0);
    grafo.insertEdge(7, 1);
    grafo.insertEdge(2, 0);
    grafo.insertEdge(3, 0);
    grafo.insertEdge(4, 0);
    grafo.insertEdge(5, 0);

    grafo.generateDot("Grafo_Original", result_dir_path);

    Graph *newGraph = grafo.directTransitiveClosure(0);
    newGraph->generateDot("Teste_direct_transitive_closure", result_dir_path);
    delete newGraph;
    newGraph = grafo.indirectTransitiveClosure(0);
    newGraph->generateDot("Teste_indirect_transitive_closure", result_dir_path);
    delete newGraph;
    Graph grafo2(7, false, true, false);

    grafo2.insertNode(0);
    grafo2.insertNode(1);
    grafo2.insertNode(2);
    grafo2.insertNode(3);
    grafo2.insertNode(4);
    grafo2.insertNode(5);
    grafo2.insertNode(6);

    grafo2.insertEdge(0, 1);
    grafo2.insertEdge(0, 2);
    grafo2.insertEdge(0, 3);
    grafo2.insertEdge(1, 2);
    grafo2.insertEdge(4, 1, 10);
    grafo2.insertEdge(2, 5);
    grafo2.insertEdge(5, 4);

    grafo2.generateDot("grafo2", result_dir_path);

    newGraph = grafo2.breadthFirstSearch(4, result_dir_path);

    newGraph = grafo2.floydMarshall(4, 3);
    newGraph->generateDot("Menor_Caminho", result_dir_path);

    delete newGraph;

    newGraph = Utils::createGraph(args[1], stoi(args[3]), stoi(args[4]), stoi(args[5]));

    // newGraph->generateDot("graph_file", result_dir_path);
    // newGraph = newGraph->breadthFirstSearch(5);
    // newGraph->generateDot("125_graph", result_dir_path);
    // newGraph->floydMarshall(0, 0)->generateDot("test_5_28", result_dir_path);
    // newGraph->floydMarshall(0, 0)->generateDot("test_28_5", result_dir_path);
}