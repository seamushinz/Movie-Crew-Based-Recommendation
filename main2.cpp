#include "AdjacencyList.h"
#include <iostream>
#include <chrono>

using namespace std;
int main() {
    AdjacencyList a = AdjacencyList();
    cout << "testing adjacency list for the movie: West Side Story" << endl;

    auto t1 = chrono::high_resolution_clock::now();

    vector<string> similarMovies = a.findSimilar("West Side Story");

    for (const auto& similar : similarMovies) {
        cout << similar << ", ";
    }
    cout << endl;
       cout << "time for show of shows" << endl;
    auto t2 = chrono::high_resolution_clock::now();
    cout << "Total time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms" << endl;

    //    cout << "testing adjacency list for the movie: as you like it" << endl;

    // vector<string> similarMovies2 = a.findSimilar("tt0002031");

    //    cout << "time for as you like it" << endl;
    // auto t3 = chrono::high_resolution_clock::now();
    // cout << "Total time: " << chrono::duration_cast<chrono::milliseconds>(t3 - t2).count() << " ms" << endl;

    // for (const auto& similar : similarMovies2) {
    //     cout << similar << ", ";
    // }
    // cout << endl;

    //      cout << "testing adjacency list for the movie: west side story" << endl;


    // vector<string> similarMovies3 = a.findSimilar("tt0055614");

    // for (const auto& similar : similarMovies3) {
    //     cout << similar << ", ";
    // }
    // cout << endl;

    //  cout << "time for west side story" << endl;
    // auto t4 = chrono::high_resolution_clock::now();
    // cout << "Total time: " << chrono::duration_cast<chrono::milliseconds>(t4 - t3).count() << " ms" << endl;
}