#include "crow.h"
//#include "crow_all.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "AdjacencyList.h"
#include <vector>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;


using namespace std;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Movie data structure
struct MovieInfo {
    string title;
    string synopsis;
    string poster_url;
};


string url_encode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        // Keep alphanumeric and some other common characters
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << int((unsigned char)c);
        }
    }

    return escaped.str();
}

const std::string TMDB_API_KEY = "bd9a7f2333e55379da5f3cac6b56a8b3";

// returns the movie info struct
MovieInfo fetch_movie_data(const string& title)
{
    std::string readBuffer;
    std::string encoded_title = url_encode(title);
    std::string url = "https://api.themoviedb.org/3/search/movie?api_key=" + TMDB_API_KEY + "&query=" + encoded_title;

    CURL* curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    MovieInfo info;
    info.title = title;
    info.synopsis = "No synopsis available.";
    info.poster_url = "https://via.placeholder.com/300x450?text=" + url_encode(title);

    try {
        auto json_response = json::parse(readBuffer);
        if (!json_response["results"].empty()) {
            const auto& result = json_response["results"][0];
            if (result.contains("overview")) info.synopsis = result["overview"];
            if (result.contains("poster_path")) {
                info.poster_url = "https://image.tmdb.org/t/p/w500" + result["poster_path"].get<std::string>();
            }
        }
    } catch (...) {
        std::cerr << "Failed to parse TMDb response for: " << title << endl;
    }

    return info;
}


int main()
{
    crow::SimpleApp app;
    crow::mustache::set_base("templates/");

    // Build Adjacency List
    AdjacencyList dataStruc = AdjacencyList();

    // Or build hashmap (choose which one works better and comment the other one out)
    // HashMap dataStruc = HashMap();

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([](){
        auto page = crow::mustache::load_text("index.html");
        return page;
    });

    // if search was entered call the adjacency list or hashmap via dataStruc.findSimilar(string);

    // recieve returned vector of movies

    // use API to match movie data to each movie title and return as a vector with title, synposis, and image

    CROW_ROUTE(app, "/search")
    ([&dataStruc](const crow::request& req) {
        auto movieParam = req.url_params.get("movie");
        if (!movieParam) {
            return crow::response(400, "Missing movie parameter");
        }

        string query = movieParam;
        cout << "[Search Received] " << query << std::endl;

        // Dummy similar movies — replace with adjacency list or other logic later

        /// THIS IS WHERE WERE GONNA RETURN THE STRING OF SIMILAR MOVIES
        // SO INIITALIZE THE DATA STRUCTURE ON APP START, AND CALL THE DATA STRUCTURES HERE
        vector<string> similarMovies = dataStruc.findSimilar(query);

        // Build the response JSON array
        json resultJson;
        resultJson["results"] = json::array();

        for (const std::string& title : similarMovies) {
            MovieInfo data = fetch_movie_data(title);

            json movieJson;
            movieJson["title"] = data.title;
            movieJson["synopsis"] = data.synopsis;
            movieJson["poster_url"] = data.poster_url;

            resultJson["results"].push_back(movieJson);
        }

        // Convert to string and send it back
        return crow::response{resultJson.dump()};
    });

    app.port(18080).multithreaded().run();
}