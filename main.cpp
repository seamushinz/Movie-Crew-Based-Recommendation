#include "crow.h"
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <queue>
#include <set>
#include "AdjacencyList.h"
#include "HashMap.h"
using namespace std;

int main() {
    crow::SimpleApp app;
    //---------------- read movie titles file ----------------------------------------------------------------
    HashMapImplementation hashMapVersion;

    cout << "now reading df_names.csv..." << endl;
    //------- read crew file --------------------------------------------------------------------------------
    unordered_map<string, string> crewIDtoName;
    crewIDtoName.reserve(844609);
    ifstream crewFile("data/df_names.csv");
    /* format:
nconst,primaryName,birthYear,deathYear,primaryProfession,knownForTitles
nm0000001,Fred Astaire,1899.0,1987.0,"soundtrack,actor,miscellaneous","tt0053137,tt0050419,tt0072308,tt0043044"
*/
    string line = "";
    if (crewFile.is_open()) {
        getline(crewFile, line); // skip header
        while (getline(crewFile, line)) {
            size_t start = 0, end = 0;
            int col = 0;
            string crewID, name;

            while ((end = line.find(',', start)) != string::npos) {
                string token = line.substr(start, end - start);
                if (col == 0) {
                    crewID = token;
                } else if (col == 1) {
                    name = token;
                    break;
                }
                start = end + 1;
                col++;
            }

            if (!crewID.empty() && !name.empty()) {
                crewIDtoName[crewID] = name;
            }
        }
        crewFile.close();
    } else {
        cerr << "Failed to open df_names.csv" << endl;
    }
    cout << "finished reading movie file, size: " << crewIDtoName.size() << endl;
    //---------------- creating similarity map or something? ------------------------------------------------

    /*read movies from file
    //add to mostSimialar map:
    //key: movie name, value: set{Director Name, Composer, Crew...etc.}
    */
    //test thing
    //unordered_map<string, unordered_set<string>> movies;
    /*
    priority_queue<pair<int, string>> mostSimilar;
    string selectedMovie = "tt0020403";
    //is there some algo we can use for this other than just a for loop
    for (auto it = movieIDToCastInfo.begin(); it != movieIDToCastInfo.end(); ++it){
      unordered_set<string> intersection;
      if (it->first != selectedMovie){
        unordered_set<string> currentCrew = it->second;
        set_intersection(movieIDToCastInfo[selectedMovie].begin(), movieIDToCastInfo[selectedMovie].end(),
                         currentCrew.begin(), currentCrew.end(),
                         inserter(intersection, intersection.begin()));
        pair<int, string> similarityScore = {intersection.size(), it->first};
        mostSimilar.push(similarityScore);
    }
}
    cout << mostSimilar.size() << endl;
    cout << movieIDToTitle[mostSimilar.top().second] << " Score: " << mostSimilar.top().first << endl;
    cout << movieIDToTitle[selectedMovie] << " has: " << endl;
    unordered_set<string> castIDs = movieIDToCastInfo[selectedMovie];
    for (const string& castID : castIDs) {
        //check if exists
        if (crewIDtoName.count(castID)) {
            cout << crewIDtoName[castID] << endl;
        }else {
            cout << "Doesn't exist: " << castID << endl;
        }
    }
    cout << movieIDToTitle[mostSimilar.top().second] << " has: " << endl;
    castIDs = movieIDToCastInfo[mostSimilar.top().second];
    for (const string& castID : castIDs) {
        //check if exists
        if (crewIDtoName.count(castID)) {
            cout << crewIDtoName[castID] << endl;
        }else {
            cout << "Doesn't exist: " << castID << endl;
        }
    }

    //---------------- adjacency list thing ------------------------------------------------
    AdjacencyList a = AdjacencyList();
    cout << "testing adjacency list for the movie: West Side Story" << endl;

    auto adjacencyT1 = chrono::high_resolution_clock::now();

    vector<string> similarMovies = a.findSimilar("West Side Story");

    for (const auto& similar : similarMovies) {
        cout << similar << ", ";
    }
    cout << endl;
    cout << "time for show of shows" << endl;
    auto adjacencyT2 = chrono::high_resolution_clock::now();
    cout << "Adjacency time: " << chrono::duration_cast<chrono::milliseconds>(adjacencyT2 - adjacencyT1).count() << " ms" << endl;

    /*
     *use max heap to make a list of the most simiilar movies
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
