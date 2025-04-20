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

    public:
        AdjacencyList();
        vector<string> findSimilar(string movie);

};

AdjacencyList::AdjacencyList() {
    ifstream movieFile("data/df_movies.csv");
    unordered_map<string, unordered_set<string>> movieToCrew;
    unordered_map<string, unordered_set<string>> crewToMovies;
    auto t1 = chrono::high_resolution_clock::now();
    string line;
    cout << "opening file: df_movies.csv" << endl;
    if (movieFile.is_open()) {
        getline(movieFile, line); 

        while (getline(movieFile, line)) {
            size_t start = 0, end = 0;
            int col = 0;
            string movieID, title, crewID, directorID, writerID;
            unordered_set<string> castIDs;
            castIDs.reserve(3);

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

            if (col == 15) {
                writerID = line.substr(start);
            }

            if (!crewID.empty()) {
                castIDs.insert(crewID);
            }
            if (!directorID.empty()) {
                castIDs.insert(directorID);
            }
            if (!writerID.empty()) {
                castIDs.insert(writerID);
            }

            movieToCrew[movieID] = castIDs;

            for (const string& crew : castIDs) {
                crewToMovies[crew].insert(movieID);
            }
        }

        movieFile.close();
    } else {
        cout << "Failed to open movie file: movies_df.csv" << endl;
    }
    cout << "file completed reading" << endl;
    cout << "beginning adjacencyList build" << endl;
   for (const auto& movieCrew : movieToCrew) {
    string movieID = movieCrew.first;
    const unordered_set<string>& crewSet = movieCrew.second;
    unordered_map<string, int> sharedCounts;

    for (const string& crew : crewSet) {
        for (const string& otherMovie : crewToMovies[crew]) {
            if (otherMovie == movieID) continue;
            sharedCounts[otherMovie]++;
        }
    }

    for (const auto& other : sharedCounts) {
        string otherMovie = other.first;
        int count = other.second;
        adjacencyList[movieID].emplace_back(otherMovie, count);
        adjacencyList[otherMovie].emplace_back(movieID, count); // double edge for undirected adj list
    }
}

    auto t2 = chrono::high_resolution_clock::now();
    cout << "Finished building adjacency list, size: " << adjacencyList.size() << endl;
    cout << "Processing took " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms\n";

}

bool compareWeights(pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;
}

vector<string> AdjacencyList::findSimilar(string movie) {
    vector<string> similarFilms;
    if (adjacencyList.count(movie) == 0 || adjacencyList[movie].empty()) {
        return similarFilms;
    }

    vector<pair<string, int>> neighbors = adjacencyList[movie];

    sort(neighbors.begin(), neighbors.end(), compareWeights);

    for (const auto& neighbor : neighbors) {
        similarFilms.push_back(neighbor.first);
    }
    return similarFilms;
        
    }
    
