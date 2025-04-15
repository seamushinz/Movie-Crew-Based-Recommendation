#include "crow.h"

int main() {
    crow::SimpleApp app;
    std::cout << "Current path: " << std::filesystem::current_path() << std::endl;
    /*read movies from file
    //add to map:
        //key: movie name, value: set{Director Name, Composer, Crew...etc.}
    */
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