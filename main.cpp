#include <iostream>
#include "Utils.h"
#include "Graph.h"
#include "MinGapGraph.h"
#include <vector>
#include <set>

using namespace std;

// ./grafos.exe ~/Downloads/instancias2/n100d03p1i1.txt ./out 0 0 1
int main(int argc, char const *argv[])
{

    if (argc < 6)
    {
        cout << "Poucos argumentos na chamada do programa" << endl;
        cout << "Siga as instruções do arquivo README.md" << endl;
        return 1;
    }
    else if (argc > 6)
    {
        cout << "Muitos argumentos na chamada do programa" << endl;
        cout << "Siga as instruções do arquivo README.md" << endl;
    }

    ifstream graph_file;
    string word, weight;
    graph_file.open(argv[1], ios::in);

    int p; // number of partitions

    do
        graph_file >> word;
    while (word != "p");
    graph_file >> word >> p;

    graph_file.close();

    Graph *graph = Utils::createGraphPart2(argv[1], stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));

    srand(time(0));

    vector<MinGapGraph> minGapForest = Utils::greed(graph, p, 0.1);

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

        cout << partNum << ": " << diferenca << " order: " << part.order << endl;
        partNum++;

        for (Node *node = part.getFirstNode(); node != nullptr; node = node->getNextNode())
        {
            ids.insert(node->getId());
        }
    }

    cout << "=========================" << endl;
    cout << "Resultado: " << sum << endl;
    cout << "Soma das ordens: " << sumOrders << endl;
    cout << "=========================" << endl;
}
