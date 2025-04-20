using namespace std;
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

 // I DONT KNOW WHAT IM DOING HERE

class HashMap {
private:
    vector<vector<string>> map;
    double load_factor = 0.6;
    int size;
    int capacity;

    int hash(string& key) {
        return hash<string>{}(key) % capacity;
    }


public: 
    HashMap(unordered_map<string, unordered_set<string>> &movieIDToCastInfo);
};