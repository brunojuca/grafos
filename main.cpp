#include <iostream>
#include "Utils.h"
#include "Graph.h"

using namespace std;

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

    Graph *graph = Utils::createGraphPart2(argv[1], stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));

    return 0;
}
