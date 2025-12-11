#include "ArrayList.h"
#include <AppUi.h>
#include <FL/Enumerations.H>
#include <FL/Fl_Scroll.H>
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/button.h>
#include <bobcat_ui/dropdown.h>
#include <bobcat_ui/textbox.h>
#include <bobcat_ui/window.h>
#include <string>
#include <fstream>
#include <ios>
#include <iostream>
#include <filesystem>
#include <string>

using namespace bobcat;
using namespace std;

AppUi::AppUi() {
    window = new Window(100, 100, 400, 400, "Flight Planner");

    start = new Dropdown(20, 40, 360, 25, "Starting Point");
    destination = new Dropdown(20, 100, 360, 25, "Destination");

    for (int i = 0; i < vertexes.size(); i++) {
        start->add(vertexes[i]->data);
        destination->add(vertexes[i]->data);
    }

    search = new Button(20, 150, 170, 25, "Search");
    clear = new Button(200, 150, 170, 25, "Clear");
    ON_CLICK(search, AppUi::onClick);
    ON_CLICK(clear, AppUi::onClear);

    results = new Fl_Scroll(20, 200, 360, 180, "Results");
    results->align(FL_ALIGN_TOP_LEFT);
    results->box(FL_THIN_UP_BOX);

    window->show();

    init();
}

void AppUi::onClick(bobcat::Widget *sender) {

}

void AppUi::onClear(bobcat::Widget *sender) {

}

void AppUi::init() {
    //Reading files
    fstream filePointer_ver;
    fstream filePointer_edg;
    filePointer_ver.open("assets/vertices.csv", ios::in);
    //std::cout << filesystem::current_path() << std::endl; //Debug, I didn't know where the directory started
    if (filePointer_ver.is_open()) {
        string theText;
        while (getline(filePointer_ver, theText)) {
            vertexes.append(new Vertex(theText));
        }
    }
    filePointer_ver.close();
    filePointer_edg.open("assets/edges.csv", ios::in);
    if (filePointer_edg.is_open()) {
        string theText;
        while (getline(filePointer_edg, theText)) {
            stringstream line(theText);
            string current;
            ArrayList<int> data;
            while (getline(line,current,',')) {
                data.append(stoi(current));
            }
            edges.append(new edgeInfo(data[0],data[1],data[2],data[3]));
        }
    }
    filePointer_edg.close();

    // Adding edges and vertexes
    Graph g;
    for (int i = 0; i < vertexes.size(); i++) {
        g.addVertex(vertexes[i]);
    }
    for (int i = 0; i < edges.size(); i++) {
        edgeInfo *edge = edges[i];
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
}


