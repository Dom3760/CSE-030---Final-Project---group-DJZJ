#include "Enums.h"
#include <Graph.h>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct edgeInfo {
    int vertex1;
    int vertex2;
    int distance;
    int cost;

    edgeInfo(int Vertex1, int Vertex2, int Distance, int Cost) {
        vertex1 = Vertex1;
        vertex2 = Vertex2;
        distance = Distance;
        cost = Cost;
    }
};

struct UserFlightRequest {
    std::string startName;
    std::string endName;
    int preferenceChoice;
};

UserFlightRequest getUserInput() {
    UserFlightRequest request;
    std::string confirmation;
    std::string preference;

    while (confirmation != "y") {
        std::cout << "Please state desired Destination." << std::endl;
    std:
        getline(std::cin, request.endName);

        std::cout << "What is your starting location?" << std::endl;
        std::getline(std::cin, request.startName);

        std::cout << "What is your preference (1-Cheapest, 2-Shortest Time, 3- "
                     "Least Stops )?"
                  << std::endl;
        std::getline(std::cin, preference);

        std::cout << "you inputted:" << endl;
        std::cout << "Starting Location: " << request.startName << endl;
        std::cout << "Destination: " << request.endName << endl;
        std::cout << "Preference: "
                  << "( " << preference << " )" << endl;
        std::cout << "Does this look correct (y/n)?" << endl;
        std::getline(std::cin, confirmation);

        request.preferenceChoice = std::stoi(preference);
    }
    return request;
}

Vertex *getVertexByName(const std::vector<Vertex *> &vertices,
                        const std::string &name) {
    for (Vertex *v : vertices) {
        if (v->data == name) {
            return v;
        }
    }
    return nullptr;
}

int main() {
    vector<Vertex *> vertexes;
    vector<edgeInfo *> edges;

    // Reading files
    fstream filePointer_ver;
    fstream filePointer_edg;
    filePointer_ver.open("assets/vertices.csv", ios::in);
    // std::cout << filesystem::current_path() << std::endl; //Debug, I didn't
    // know where the directory started
    if (filePointer_ver.is_open()) {
        string theText;
        while (getline(filePointer_ver, theText)) {
            vertexes.push_back(new Vertex(theText));
        }
    }
    filePointer_ver.close();
    filePointer_edg.open("assets/edges.csv", ios::in);
    if (filePointer_edg.is_open()) {
        string theText;
        while (getline(filePointer_edg, theText)) {
            stringstream line(theText);
            string current;
            vector<int> data;
            while (getline(line, current, ',')) {
                data.push_back(stoi(current));
            }
            edges.push_back(new edgeInfo(data[0], data[1], data[2], data[3]));
        }
    }
    filePointer_edg.close();

    // Adding edges and vertexes
    Graph g;
    for (Vertex *location : vertexes) {
        g.addVertex(location);
    }
    for (edgeInfo *edge : edges) {
        g.addDirectedEdge(vertexes[edge->vertex1], vertexes[edge->vertex2],edge->distance, edge->cost);
        g.addDirectedEdge(vertexes[edge->vertex2], vertexes[edge->vertex1], edge->distance, edge->cost);
    }

    UserFlightRequest request = getUserInput();

    Vertex *startVertex = getVertexByName(vertexes, request.startName);
    Vertex *endVertex = getVertexByName(vertexes, request.endName);

    //TEMPORARY/change
    if (startVertex == nullptr) {
    std::cout << "\nERROR: Starting airport '" << request.startName << "' not found in data." << std::endl;
    return 1;
    }
    if (endVertex == nullptr) {
        std::cout << "ERROR: Destination airport '" << request.endName << "' not found in data." << std::endl;
        return 1;
    }



    Optimization preference;

    if (request.preferenceChoice == 1)
        preference = CHEAPEST;
    else if (request.preferenceChoice == 2)
        preference = SHORTEST_TIME;
    else
        preference = LEAST_STOPS;

    Waypoint *path = g.findOptimalPath(startVertex, endVertex, preference);

    if (path) {
        std::cout << "optimal flight itenerary (preference: "
                  << request.preferenceChoice << "):" << std::endl;

        std::cout << "Total Price: $" << path->totalCost << std::endl;
        std::cout << "Total Time: " << path->totalTime << " minutes"
                  << std::endl;
        std::cout << "Number of stops: " << path->totalStops << std::endl;
    } else {
        std::cout << "there is not path between " << request.startName
                  << " and " << request.endName << std::endl;
    }

    for(Vertex* v : vertexes)
    {
        delete v;
    }
    for(edgeInfo* e :edges)
    {
        delete e;
    }

    return 0;
}