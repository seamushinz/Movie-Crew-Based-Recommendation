#include "crow.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
using namespace std;

int main() {
    crow::SimpleApp app;
    std::cout << "Current path: " << std::filesystem::current_path() << std::endl;

    unordered_map<string, unordered_set<string>> movies;
    priority_queue<pair<int, string>> mostSimilar;
    string selectedMovie;
    /*read movies from file
    //add to map:
        //key: movie name, value: set{Director Name, Composer, Crew...etc.}
    */
    for (unordered_map<string, unordered_set<string>>::iterator it = movies.begin(); it != movies.end(); ++it){
      set<string> intersection;
      if (it->first != selectedMovie){
        unordered_set<string> currentCrew = it->second;
        set_intersection(movies[selectedMovie].begin(), movies[selectedMovie].end(),
                         currentCrew.begin(), currentCrew.end(),
                         inserter(intersection, intersection.begin()));
        pair<int, string> similarityScore = {intersection.size(), it->first};
        mostSimilar.push(similarityScore);
    }
    /*
     *use max heap to make a list of the most simiilar movies
     *using comparisons of the sets via intersection size of the sets
     */
    // Set template directory (relative to executable's working directory)
    //crow::mustache::set_base("/Users/seamushinz/Documents/school/Project 3 DSA/templates");

    CROW_ROUTE(app, "/")([](){
        // Use load() instead of load_text() for Mustache templates
        auto page = crow::mustache::load_unsafe("testPage.html");
        crow::mustache::context ctx;  // Add context even if empty
        return page.render(ctx);
    });

    app.port(18080).multithreaded().run();
}