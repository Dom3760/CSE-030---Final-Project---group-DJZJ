#include <Graph.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <filesystem>
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

int main(){
    vector<Vertex *> vertexes;
    vector<edgeInfo *> edges;

    //Reading files
    fstream filePointer_ver;
    fstream filePointer_edg;
    filePointer_ver.open("assets/vertices.csv", ios::in);
    //std::cout << filesystem::current_path() << std::endl; //Debug, I didn't know where the directory started
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
            while (getline(line,current,',')) {
                data.push_back(stoi(current));
            }
            edges.push_back(new edgeInfo(data[0],data[1],data[2],data[3]));
        }
    }
    filePointer_edg.close();

    // Adding edges and vertexes
    Graph g;
    for (Vertex *location:vertexes) {
        g.addVertex(location);
    }
    for (edgeInfo *edge:edges) {
        g.addEdge(vertexes[edge->vertex1],vertexes[edge->vertex2],edge->distance);
    }

    // Vertex* london = new Vertex("London");
    // g.addEdge(nyc, london, 6);
    // g.addEdge(london, moscow, 6););

    Waypoint* path = g.ucs(vertexes[0], vertexes[1]);

    if (path){
        cout << "We found a path" << endl;
        Waypoint* temp = path;
        while (temp != nullptr){
            cout << temp->vertex->data << " " << temp->partialCost << endl;
            temp = temp->parent;
        }
    }
    else{
        cout << "There is no path" << endl;
        //test23
    }

    
    return 0;
}