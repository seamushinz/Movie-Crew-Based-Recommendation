#pragma once
#include <fstream>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <string>
using namespace std;

template <typename t, typename b>
class HashMap {
    //use optional<> to tell which slots are empty more easily
    vector<optional<pair<t, b>>> hashMap;
    const double load_factor = 0.5;
    size_t size;
    size_t capacity;

    public:
        HashMap() {
            size = 0;
            capacity = 10;
            hashMap.resize(capacity);
        }
        int hash(const string& key) const {
            //given movie ID as key
            //sum all ascii characters
            //djb2 inspired hash function?
            int sum = 0;
            for (const char c : key) {
                sum += (sum << 7) + c;
            }
            return (sum) % capacity;
        }
        void rehash() {
            //cout << "Current capacity before rehash: " << capacity << endl;
            //cout << "Size: " << size << endl;
            vector<optional<pair<t,b>>> oldMap = hashMap;
            capacity *= 2;
            cout << "Successfully copied oldMap, new size: "<< size << " capacity: " << capacity << endl;
            hashMap.clear();
            hashMap.resize(capacity);
            size = 0;
            for (int i = 0; i < oldMap.size(); i++) {
                if (oldMap[i].has_value()) {
                    insertNoRehash(oldMap[i]->first, oldMap[i]->second);
                }
            }
        }
        void insertNoRehash(t key, b value) {
            int index = hash(key);
            while (hashMap[index].has_value()) {
                index = (index + 1) % capacity;
            }
            hashMap[index] = {key, value};
            size++;
        }

        void insert(t key, b value) {
            //check if there's space
            if ((size+1.0)/capacity > load_factor) {
                //rehash and expand
                rehash();
            }
            size++;
            int index = hash(key);
            //linear probing for filled buckets
            while (hashMap[index].has_value()) {
                //has wraparound
                if (hashMap[index]->first == key) {
                    hashMap[index]->second = value;
                }
                index = (index + 1) % capacity;
            }
            hashMap[index] = {key, value};
        }
        b getValue(t key) {
            int index = hash(key);
            while (hashMap[index].has_value()) {
                if (hashMap[index]->first == key) {
                    return hashMap[index]->second;
                }
                index = (index + 1) % capacity;
            }
            cout << "key not found: " << key << endl;
            return b();
        }
    //returns the pair at the raw index no hashing
        optional<pair<t,b>> getPairIndex(int i) {
            return hashMap[i];
        }
        size_t getSize() const {
            return size;
        }
        size_t getCapacity() const {
            return capacity;
        }
};

class HashMapImplementation {
    // key = ID, value = {id, Movie name}
    HashMap<string, string> movieIDToTitle;
    HashMap<string, string> titleToMovieID;
    // key = ID, value = set of cast IDs
    HashMap<string, unordered_set<string>> movieIDToCastInfo;

    HashMap<string, string> crewIDtoName;
    const int similarMoviesToReturn = 10;
    const int maxSize = 600000;
    const string pathToMovies = "../data/df_movies.csv";
    const string pathToNames = "../data/df_names.csv";
public:
    explicit HashMapImplementation();
    string resolveTitletoID(string title);
    vector<string> createSimilarityThing(const string& selectedMovieID);
};

inline string HashMapImplementation::resolveTitletoID(string title) {

}

inline vector<string> HashMapImplementation::createSimilarityThing(const string& selectedMovieID) {
    priority_queue<pair<int, string>> mostSimilar;
    unordered_set<string> selectedMovieCastInfo = movieIDToCastInfo.getValue(selectedMovieID);
    //is there some algo we can use for this other than just a for loop
    for (int i = 0; i < movieIDToCastInfo.getSize(); i++){
        unordered_set<string> intersection;
        auto thisMovieCastPair =  movieIDToCastInfo.getPairIndex(i);
        if (!thisMovieCastPair.has_value()) {break;}
        pair<string, unordered_set<string>> item = thisMovieCastPair.value();
        if (item.first != selectedMovieID){
            unordered_set<string> currentCrew = item.second;
            set_intersection(selectedMovieCastInfo.begin(), selectedMovieCastInfo.end(),
                             currentCrew.begin(), currentCrew.end(),
                             inserter(intersection, intersection.begin()));
            pair<int, string> similarityScore = {intersection.size(), item.first};
            mostSimilar.push(similarityScore);
        }
    }
    cout << mostSimilar.size() << endl;
    cout << movieIDToTitle.getValue(mostSimilar.top().second) << " Score: " << mostSimilar.top().first << endl;
    cout << movieIDToTitle.getValue(selectedMovieID) << " has: " << endl;
    unordered_set<string> castIDs = movieIDToCastInfo.getValue(selectedMovieID);

    for (const string& castID : castIDs) {
        //check if exists
        if (crewIDtoName.getValue(castID).data()) {
            cout << crewIDtoName.getValue(castID)<< endl;
        }else {
            cout << "Doesn't exist: " << castID << endl;
        }
    }
    cout << movieIDToTitle.getValue(mostSimilar.top().second) << " has: " << endl;
    castIDs = movieIDToCastInfo.getValue(mostSimilar.top().second);
    for (const string& castID : castIDs) {
        //check if exists
        if (crewIDtoName.getValue(castID).data()) {
            cout << crewIDtoName.getValue(castID) << endl;
        }else {
            cout << "Doesn't exist: " << castID << endl;
        }
    }
    vector<string> mostSimilarMoviesNames;
    //return a vector of 10 most similar movies
    mostSimilarMoviesNames.reserve(similarMoviesToReturn);
    cout << "most similar movies: " << endl;
    for (int i = 0; i < similarMoviesToReturn; i++) {
        string thing = movieIDToTitle.getValue(mostSimilar.top().second);
        if (find(mostSimilarMoviesNames.begin(), mostSimilarMoviesNames.end(), thing) == mostSimilarMoviesNames.end()) {
            mostSimilarMoviesNames.push_back(thing);
        }
    }
    return mostSimilarMoviesNames;
}


inline HashMapImplementation::HashMapImplementation() {
    //---------------- read movie titles file ----------------------------------------------------------------
    ifstream movieFile(pathToMovies);
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
        while (getline(movieFile, line) && movieIDToTitle.getSize() < maxSize) {
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
            movieIDToCastInfo.insert(movieID,castIDs);
            movieIDToTitle.insert(movieID,title);
            titleToMovieID.insert(title,movieID);
        }
        movieFile.close();
    } else {
        cerr << "Failed to open df_movies.csv" << endl;
    }
    cout << "finished reading movie file, size: " << movieIDToTitle.getSize() << endl;
    cout << "sample:" << endl;
    cout << "Movie title: "<< movieIDToTitle.getValue("tt0002031") << endl;
    //---------------- read crew names file ----------------------------------------------------------------
    ifstream crewFile(pathToNames);
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
                crewIDtoName.insert(crewID,name);
            }
        }
        crewFile.close();
    } else {
        cerr << "Failed to open df_names.csv" << endl;
    }
    cout << "finished reading crew names file, size: " << crewIDtoName.getSize() << endl;
    auto t2 = chrono::high_resolution_clock::now();
    cout << "processing took " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms\n";
}