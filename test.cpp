#include "crow.h"
#include <unordered_set>
#include <iostream>
#include "AdjacencyList.h"
#include "HashMap.h"
using namespace std;

int main() {
    crow::SimpleApp app;
    //---------------- read movie titles file ----------------------------------------------------------------
    auto hashMapT1 = chrono::high_resolution_clock::now();
    HashMapImplementation hashMapVersion;

    //---------------- creating similarity map ------------------------------------------------------
    vector<string> similarHashMovies = hashMapVersion.createSimilarityThing(hashMapVersion.resolveTitletoID("West Side Story"));
    cout << "hash map similar movies: ";
    for (const auto & i : similarHashMovies) {
        cout << i << ", ";
    }

    auto hashMapT2 = chrono::high_resolution_clock ::now();

    cout << "Hash map time: " << chrono::duration_cast<chrono::milliseconds>(hashMapT2 - hashMapT1).count() << " ms" << endl;

    //---------------- adjacency list version  ------------------------------------------------
    auto adjacencyT1 = chrono::high_resolution_clock::now();

    AdjacencyList a = AdjacencyList();
    cout << "testing adjacency list for the movie: West Side Story" << endl;

    vector<string> similarMovies = a.findSimilar("West Side Story");

    for (const auto& similar : similarMovies) {
        cout << similar << ", ";
    }
    cout << endl;
    cout << "time for show of shows" << endl;
    auto adjacencyT2 = chrono::high_resolution_clock::now();
    cout << "Adjacency time: " << chrono::duration_cast<chrono::milliseconds>(adjacencyT2 - adjacencyT1).count() << " ms" << endl;

    /*
     *use max heap to make a list of the most similar movies
     *using comparisons of the sets via intersection size of the sets
     */
    // Set template directory (relative to executable's working directory)
    //crow::mustache::set_base("/Users/seamushinz/Documents/school/Project 3 DSA/templates");


    /*CROW_ROUTE(app, "/")([](){
        // Use load() instead of load_text() for Mustache templates
        auto page = crow::mustache::load_unsafe("testPage.html");
        crow::mustache::context ctx;  // Add context even if empty
        return page.render(ctx);
    });

    app.port(18080).multithreaded().run();
    */
}
