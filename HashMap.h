#include <fstream>
#include <vector>
#include <unordered_set>
#include <functional>
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
            //djb2 hash function?
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
        int getSize() {
            return size;
        }
};

class HashMapImplementation {
    // key = ID, value = {id, Movie name}
    HashMap<string, string> movieIDToTitle;
    HashMap<string, string> titleToMovieID;
    // key = ID, value = set of cast IDs
    HashMap<string, unordered_set<string>> movieIDToCastInfo;

public:
    explicit HashMapImplementation();
};

inline HashMapImplementation::HashMapImplementation() {
    //---------------- read movie titles file ----------------------------------------------------------------
    ifstream movieFile("../data/df_movies.csv");
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
        while (getline(movieFile, line) && movieIDToTitle.getSize() < 600000) {
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
    auto t2 = chrono::high_resolution_clock::now();
    cout << "processing took " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " ms\n";

    cout << "sample:" << endl;
    cout << "Movie title: "<< movieIDToTitle.getValue("tt0002031") << endl;

}
