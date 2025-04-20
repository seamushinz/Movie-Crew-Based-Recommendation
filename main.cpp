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
using namespace std;

int main() {
    crow::SimpleApp app;
    //read movie titles file
    ifstream movieFile("data/df_movies.csv");
    unordered_map<string, unordered_set<string>> movieIDToCastInfo;
    unordered_map<string, string> movieIDToTitle;
    unordered_map<string, pair(double, int)> movieIDtoRating;
    //reserve size for maps, since we know how big the map will be
    movieIDToCastInfo.reserve(247560);
    movieIDToTitle.reserve(247560);
    // movieTitleToID.reserve(247560);
    cout << "reading df_movies.csv..." << endl;
    string line;
    //format:
    /*
,ID_title,titleType,primaryTitle,originalTitle,startYear,runtimeMinutes,genres,averageRating,numVotes,ID_crew,category,job,characters,director,writer
0,tt0000009,movie,Miss Jerry,Miss Jerry,1894.0,45.0,Romance,5.3,88,nm0063086,actress,,"[""Miss Geraldine Holbrook (Miss Jerry)""]",nm0085156,nm0085156
     */
    //check processing time
    auto t1 = chrono::high_resolution_clock::now();
    if (movieFile.is_open()) {
        getline(movieFile, line); //skip header
        while (getline(movieFile, line)) {
            size_t start = 0, end = 0;
            int col = 0;
            string movieID, title, crewID, directorID, writerID;
            //need sets for intersection operations later
            unordered_set<string> castIDs;
            castIDs.reserve(3);
            //this method of csv parsing is apparently faster than using getline()
            while ((end = line.find(',', start)) != string::npos) {
                string currentData = line.substr(start, end - start);
                switch (col) {
                    case 1:
                        movieID = currentData;
                    break;
                    case 3:
                        title = currentData;
                    break;
                    case 10:
                        crewID = currentData;
                    break;
                    case 14:
                        directorID = currentData;
                    break;
                    case 15:
                        writerID = currentData;
                    break;
                    default:
                        break;
                }
                start = end + 1;
                col++;
            }
            // get last field if no trailing comma
            if (col == 15) {
                writerID = line.substr(start);
            }

            if (!directorID.empty()) {castIDs.insert(directorID);}
            if (!writerID.empty()) {castIDs.insert(writerID);}
            if (!crewID.empty()) {castIDs.insert(crewID);}
            movieIDToCastInfo[movieID] = castIDs;
            movieIDToTitle[movieID] = title;
            // movieTitleToID[title] = movieID;
        }
        movieFile.close();
    } else {
        cerr << "Failed to open df_movies.csv" << endl;
    }
    cout << "finished reading movie file, size: " << movieIDToTitle.size() << endl;
    auto t2 = chrono::high_resolution_clock::now();
    cout << "processing took " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms\n";

    cout << "sample:" << endl;
    cout << "Movie title: "<< movieIDToTitle["tt0020403"] << endl;

    cout << "now reading df_names.csv..." << endl;
    //read crew file
    unordered_map<string, string> crewIDtoName;
    crewIDtoName.reserve(844609);
    ifstream crewFile("data/df_names.csv");
    /* format:
nconst,primaryName,birthYear,deathYear,primaryProfession,knownForTitles
nm0000001,Fred Astaire,1899.0,1987.0,"soundtrack,actor,miscellaneous","tt0053137,tt0050419,tt0072308,tt0043044"
*/
    line = "";
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
    cout << "sample:" << endl;

    cout << "Cast of " << movieIDToTitle["tt0020403"] << ": " << endl;
    unordered_set<string> castIDs = movieIDToCastInfo["tt0020403"];
    for (const string& castID : castIDs) {
        //check if exists
        if (crewIDtoName.count(castID)) {
            cout << crewIDtoName[castID] << endl;
        }else {
            cout << "Doesn't exist: " << castID << endl;
        }
    }

    /*read movies from file
    //add to map:
    //key: movie name, value: set{Director Name, Composer, Crew...etc.}
    */
    unordered_map<string, unordered_set<string>> movies;
    priority_queue<pair<int, string>> mostSimilar;
    string selectedMovie;

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


    /*CROW_ROUTE(app, "/")([](){
        // Use load() instead of load_text() for Mustache templates
        auto page = crow::mustache::load_unsafe("testPage.html");
        crow::mustache::context ctx;  // Add context even if empty
        return page.render(ctx);
    });

    app.port(18080).multithreaded().run();
    */
}
