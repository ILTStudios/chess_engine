#include <iostream>
#include "game.h" // includes the game header
#include "json.hpp" // helps handle post and request data
#include "httplib.h" // helps connect to an http server

using namespace std;
using json = nlohmann::json;

int main(){
    Game game;
    game.initialise();

    httplib::Server server;

    // handles get request
    server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("Chess server is running!", "text/plain");
    });
    //

    // handles the stupid CORS bs
    server.Options("/move", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    });
    //

    // handles receiving a move
    server.Post("/move", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        try {
            json data = json::parse(req.body);
            if (!data.contains("from") || !data.contains("to") ||
                !data["from"].is_number_integer() ||
                !data["to"].is_number_integer()) {
                res.status = 400;
                res.set_content(
                    R"({"success":false,"message":"Invalid request"})", "application/json"
                );
                return;
            }

            int from = data["from"];
            int to = data["to"];
            cout << "Received move: " << from << " -> " << to << endl;

            // do all the game move stuff

            json response = {{"success", true}, {"message", "Move received"}};
            res.set_content(response.dump(), "application/json");
        } catch (const exception& e) { // catch any errors
            res.status = 400;
            json response = {{"success", false}, {"message", "Invalid JSON"}};
            res.set_content(response.dump(), "application/json");
        }
    });

    //

    // starting the server now
    cout << "Starting chess server on port 8000..." << endl;
    server.listen("127.0.0.1", 8000);
    return 0;
    //
}