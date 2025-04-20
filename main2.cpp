#include "AdjacencyList.h"
#include <iostream>
using namespace std;
int main() {
    cout << "testing adjacency list for the movie: Julius Caesar" << endl;
    AdjacencyList a = AdjacencyList();
    vector<string> similarMovies = a.findSimilar("tt0004022");

    // for (const auto& similar : similarMovies) {
    //     cout << similar << ", ";
    // }
    cout << endl;
}