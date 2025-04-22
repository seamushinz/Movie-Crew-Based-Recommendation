#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <set>
#include <chrono>
#include <iostream>
using namespace std;
class AdjacencyList {
    private:
        unordered_map<string, vector<pair<string, int>>> adjacencyList;
        unordered_map<string, string> movieIDtoTitle;
        unordered_map<string, vector<string>> movieTitletoIDs;


    public:
        AdjacencyList();
        vector<string> findSimilar(string movie);
        string resolveTitletoID(string title);

};
// comp to order weights
bool compareWeights(pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;
}

// call this is a movie entered in by the user has two ID's associated with it (gotta find the best one)
string AdjacencyList::resolveTitletoID(string title) {
    if (movieTitletoIDs.count(title) == 0) {
        return "";
    }

    string bestID = "";
    int max = -100000;

    vector<string> possibleIDs = movieTitletoIDs[title];
    for (const auto& id : possibleIDs) {
        int neighbors = 0;
        if (adjacencyList.count(id)) {
            neighbors = adjacencyList[id].size();
            // num of neighbors = list of edges at adjacency list for ID
        }

        // choose the one with the most neighbors (assume user meant most popular version of the movie)
        if (neighbors > max) {
            max = neighbors;
            bestID = id;
        }
    }
    return bestID;
}

AdjacencyList::AdjacencyList() {
    auto t1 = chrono::high_resolution_clock::now();

    unordered_map<string, unordered_map<string, int>> edgeList;
    string line;

    ifstream movieFile("data/df_movies.csv");
    cout << "loading df_movies.csv to map titles to their ID's ..." << endl;
    if (movieFile.is_open()) {
        getline(movieFile, line);
        while (getline(movieFile, line)) {
            stringstream ss(line);
            string token;
            string movieID, title;
            int col = 0;

            while (getline(ss, token, ',')) {
                if (col == 1) movieID = token;
                if (col == 3) title = token;
                col++;
            }

            if (!movieID.empty() && !title.empty() && movieIDtoTitle.count(movieID) == 0) {
                movieIDtoTitle[movieID] = title;
                movieTitletoIDs[title].push_back(movieID); // reverse mapping (also putting ID's if they have duplicate names at one position)

            }
        }
        movieFile.close();
    } else {
        cout << "failed to open df_movies.csv for mapping titles" << endl;
    }

    cout << "valid movies loaded: " << movieIDtoTitle.size() << endl;

    ifstream nameFile("data/df_names.csv");

    cout << "opening file: df_names.csv" << endl;
    if (nameFile.is_open()) {
        getline(nameFile, line); 
        // thank you Seaumus for giving line reading code I would've never figured it out

        while (getline(nameFile, line)) {
            size_t start = 0;
            size_t end = 0;
            int col = 0;
            string personID, primaryName, birthYear, deathYear, profession, knownForTitles;

            while ((end = line.find(',', start)) != string::npos && col < 5) {
                string currentData = line.substr(start, end - start);
                switch (col) {
                    case 0: 
                        personID = currentData; 
                        break;
                    case 1: 
                        primaryName = currentData; 
                        break;
                    case 2: 
                        birthYear = currentData; 
                        break;
                    case 3: 
                        deathYear = currentData; 
                        break;
                    case 4: 
                        profession = currentData; 
                        break;
                    default: 
                        break;
                }
                start = end + 1;
                col++;
            }

            if (col == 5) {
                knownForTitles = line.substr(start);
            }

            if (knownForTitles.empty()) {
                continue;
            }

            vector<string> titles;
            size_t pos = 0;
            while ((pos = knownForTitles.find(',')) != string::npos) {
                string currTitle = knownForTitles.substr(0, pos);
                if (movieIDtoTitle.count(currTitle)) {
                    titles.push_back(currTitle);
                }
                knownForTitles.erase(0, pos + 1);
            }
            if (!knownForTitles.empty() && movieIDtoTitle.count(knownForTitles)) {
                titles.push_back(knownForTitles);
            }

            if (titles.size() < 2) {
                continue;
            }

            for (size_t i = 0; i < titles.size(); i++) {
                for (size_t j = i + 1; j < titles.size(); j++) {
                    string curr_movie = titles[i];
                    string compare_movie = titles[j];
                    edgeList[curr_movie][compare_movie]++;
                    edgeList[compare_movie][curr_movie]++;
                }
            }
        }

        nameFile.close();
    } else {
        cout << "Failed to open name file: df_names.csv" << endl;
    }



    cout << "finished parsing df_names.csv and built edge list" << endl;
    auto t2 = chrono::high_resolution_clock::now();
    cout << "Total time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms" << endl;

for (const auto& entry : edgeList) {
    string movieID = entry.first;
    vector<pair<string, int>> neighbors(entry.second.begin(), entry.second.end());

    sort(neighbors.begin(), neighbors.end(), compareWeights);

    if (neighbors.empty()) continue;

    if (neighbors.size() > 10) {
        neighbors.resize(10);
    }

    for (const auto& neighbor : neighbors) {
        adjacencyList[movieID].emplace_back(neighbor.first, neighbor.second);
    }
}


    cout << "adjacency list built, size: " << adjacencyList.size() << endl;

  
    auto t3 = chrono::high_resolution_clock::now();
    cout << "Total processing time building adjacency list: " << chrono::duration_cast<chrono::milliseconds>(t3 - t2).count() << " ms" << endl;
}



vector<string> AdjacencyList::findSimilar(string movie) {
    vector<string> similarFilms;
    
    string ID = resolveTitletoID(movie);

    if (adjacencyList.count(ID) == 0 || adjacencyList[ID].empty()) {
        return similarFilms;
    }

    vector<pair<string, int>> neighbors = adjacencyList[ID];

    sort(neighbors.begin(), neighbors.end(), compareWeights);

    for (const auto& neighbor : neighbors) {
        similarFilms.push_back(movieIDtoTitle[neighbor.first]);
    }
    return similarFilms;
        
    }
    
