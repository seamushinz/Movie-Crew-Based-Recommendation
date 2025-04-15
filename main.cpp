#include "crow.h"

int main() {
    crow::SimpleApp app;
    std::cout << "Current path: " << std::filesystem::current_path() << std::endl;

    //I can't get this shit to work
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