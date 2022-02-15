#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <ctime>
#include "Utils.h"
#include "Graph.h"
#include "MinGapGraph.h"
#include <vector>
#include <set>
#include <limits>
// #include <processthreadsapi.h>

using namespace std;

// static double get_cpu_time(){
//     FILETIME a,b,c,d;
//     if (GetProcessTimes(GetCurrentProcess(),&a,&b,&c,&d) != 0){
//         //  Returns total user time.
//         //  Can be tweaked to include kernel times as well.
//         return
//             (double)(d.dwLowDateTime |
//             ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
//     }else{
//         //  Handle erro
//         return 0;
//     }
// }

// ./grafos.exe instances/n100d03p1i1.txt ./out 0 0 1
int main(int argc, char const *argv[])
{

    if (argc < 4)
    {
        cout << "Poucos argumentos na chamada do programa" << endl;
        cout << "Siga as instruções do arquivo README.md" << endl;
        return 1;
    }
    else if (argc > 4)
    {
        cout << "Muitos argumentos na chamada do programa" << endl;
        cout << "Siga as instruções do arquivo README.md" << endl;
    }

    ifstream graph_file;
    string word, weight;

    int p; // number of partitions

    string files[4] = {
                        // "n100d03p1i1",
                        // "n100plap1i1",
                        // "n200d03p3i1",
                        // "n200plap1i1",
                        // "n200plap1i3",
                        // "n300d03p1i5",
                        "n300plap1i1",
                        "n400plap1i5",
                        "n500d06p3i3",
                        "n500plap3i5"
                        };

    for (int i = 0; i < 4; i++)
    {
        graph_file.open("instances/" + files[i] + ".txt", ios::in);
        do
            graph_file >> word;
        while (word != "p");
        graph_file >> word >> p;

        graph_file.close();

        Graph *graph = Utils::createGraphPart2("instances/" + files[i] + ".txt", stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));

        srand(time(NULL));

        cout << "Arquivo: " << files[i] << endl
             << endl;

        clock_t start = clock();
        vector<MinGapGraph> minGapForest = Utils::greed(graph, p);
        clock_t end = clock();

        cout << endl;

        int sum = 0;
        int sumOrders = 0;
        int partNum = 1;
        set<int> ids;
        for (auto &&part : minGapForest)
        {
            int diferenca = part.maxNodeWeight - part.minNodeWeight;
            sum += diferenca;
            sumOrders += part.order;

            partNum++;

            for (Node *node = part.getFirstNode(); node != nullptr; node = node->getNextNode())
            {
                ids.insert(node->getId());
            }
        }

        long double time_elapsed = 1000.0 * (end - start) / CLOCKS_PER_SEC;

        cout << "Resultado: " << sum << endl;
        cout << "Soma das ordens: " << sumOrders << endl;
        cout << "=========================" << endl;

        ofstream result_file("resultados/resultados_a1.txt", ios::out | ios::app);
        result_file << "Resultados A1 para " << files[i] << ":" << endl;
        result_file << "Resultado " << std::setfill('.') << std::setw(20) << " " + to_string(sum) << endl;
        result_file << "Tempo     " << std::setfill('.') << std::setw(20) << " " + to_string(time_elapsed) + "ms" << endl;
        result_file << endl
                    << endl;

        delete graph;
        result_file.close();
    }
    float alphas[3] = {0.1, 0.2, 0.3};

    for (int k = 0; k < 4; k++)
    {
        for (auto &&alpha : alphas)
        {
            for (int i = 0; i < 30; i++)
            {
                int best_result = std::numeric_limits<int>::max();
                long double time_elapsed;
                cout << endl << "Progresso época " << (i + 1) << ":" << endl;
                for (int j = 0; j < 500; j++)
                {
                    graph_file.open("instances/" + files[k] + ".txt", ios::in);
                    do
                        graph_file >> word;
                    while (word != "p");
                    graph_file >> word >> p;

                    graph_file.close();

                    Graph *graph = Utils::createGraphPart2("instances/" + files[k] + ".txt", stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));
                    
                    srand(time(NULL));

                    clock_t start = clock();
                    vector<MinGapGraph> minGapForest = Utils::greed(graph, p, alpha);

                    int sum = 0;
                    int sumOrders = 0;
                    int partNum = 1;
                    set<int> ids;
                    for (auto &&part : minGapForest)
                    {
                        int diferenca = part.maxNodeWeight - part.minNodeWeight;
                        sum += diferenca;
                        sumOrders += part.order;

                        partNum++;

                        for (Node *node = part.getFirstNode(); node != nullptr; node = node->getNextNode())
                        {
                            ids.insert(node->getId());
                        }
                    }
                    clock_t end = clock();

                    if (best_result > sum)
                    {
                        time_elapsed = 1000.0 * (end - start) / CLOCKS_PER_SEC;
                        best_result = sum;
                    }
                    cout << std::setfill('0') << std::setw(2) << "\b\b\b" << (int)(((float)(j + 1) / 500) * 100) << "%" << std::flush;
                    delete graph;
                }
                ofstream result_file("resultados/resultados_a2/" + files[k] + ".txt", ios::out | ios::app);
                result_file << "Resultados " << (i + 1) << " A2 para " << files[k] << "(Alpha: " << alpha << "):" << endl;
                result_file << "Resultado " << std::setfill('.') << std::setw(20) << " " + to_string(best_result) << endl;
                result_file << "Tempo     " << std::setfill('.') << std::setw(20) << " " + to_string(time_elapsed) + "ms" << endl;
                result_file << endl
                            << endl;
            }
        }
    }

    return 0;
}
