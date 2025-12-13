#include "ArrayList.h"
#include "Enums.h"
#include "Graph.h"
#include <AppUi.h>
#include <FL/Enumerations.H>
#include <FL/Fl_Scroll.H>
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/button.h>
#include <bobcat_ui/dropdown.h>
#include <bobcat_ui/textbox.h>
#include <bobcat_ui/window.h>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>


using namespace bobcat;
using namespace std;

struct finalResultData {
    string name;
    string txt;

    finalResultData(string gotName, string gotTxt) {
        name = gotName;
        txt = gotTxt;
    };
};

AppUi::AppUi() {
    window = new Window(100, 100, 400, 600, "Flight Planner");

    start = new Dropdown( 20, 20, 360, 25, "Starting Point");
    destination = new Dropdown(20, 70, 360, 25, "Destination");
    preference = new Dropdown(20, 120, 360, 25, "Preference");
    preference->add("Cheapest Price");
    preference->add("Shortest Travel Time");
    preference->add("Least Number of Stops");

    ON_CHANGE(start, AppUi::onSelect)
    ON_CHANGE(destination, AppUi::onSelect)
    ON_CHANGE(preference, AppUi::onSelect)
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
    if (sender != search) {
        cout << "There is no path" << endl;
        return;
    }
    // for ucs
    Waypoint *ucs = g.findOptimalPath(startNode, destNode, prefNode );
    results->clear();
    if (!ucs) {
        results->add(new TextBox(20,20, 300, 25, "There is no path."));
        cout << "There is no path" << endl;
        window->redraw();
        return;
    }
    cout << "We found a path" << endl;
    Waypoint *temp = ucs;
    ArrayList<finalResultData*> resultList;
    int y = results->y() + 10;

    while (temp != nullptr) {
        if (temp->parent != nullptr) {
            string label;
            if(prefNode == CHEAPEST){
                label = " Cost: $" + to_string(temp->partialCost);
            } else if (prefNode == SHORTEST_TIME){
                label = " Fight Time: " + to_string(temp->partialCost) + " hours";
            } else {
                label = " Stops: " + to_string(temp->partialCost);
            }
            resultList.append(new finalResultData(temp->vertex->data,label));
            cout << label << endl;
        }
        cout << temp->vertex->data << " " << temp->partialCost << endl;
        temp = temp->parent;
        window->redraw();
    }
    for (int i = resultList.size()-1;i >= 0;i--) {
        finalResultData *gotData = resultList[i];
        results->add(new TextBox(20, y, 300, 25, gotData->name));
        y += 25;
        results->add(new TextBox(20, y, 300, 25,gotData->txt));
        y += 25;
    }
}

void AppUi::onClear(bobcat::Widget *sender) {
    if (sender == clear) {
        start->value(0);
        destination->value(0);
        startNode = vertexes[0];
        destNode = vertexes[0];
        results->clear();
        window->redraw();
        cout << "cleared" << endl;
    }
}

void AppUi::updateDropdowns() {
    for (int i = 0; i < vertexes.size(); i++) {
        start->add(vertexes[i]->data);
        destination->add(vertexes[i]->data);
    }
}

void AppUi::onSelect(bobcat::Widget *sender) {
    int startindex = start->value();
    int destindex = destination->value();
    int prefindex = preference->value();

    if (sender == start) {
        startNode = vertexes[startindex];
        cout << "new start is " << startNode->data << endl;
    } else if (sender == destination) {
        destNode = vertexes[destindex];
        cout << "new destination is " << destNode->data << endl;
    } else if (sender == preference){
        if (prefindex == 0){
            prefNode = CHEAPEST;
        }
        else if (prefindex == 1){
            prefNode = SHORTEST_TIME;
        }
        else {
            prefNode = LEAST_STOPS;
        }
    }
}

void AppUi::init() {
    // Reading files
    fstream filePointer_ver;
    fstream filePointer_edg;

    // read verticies
    filePointer_ver.open("assets/vertices.csv", ios::in);
    if (filePointer_ver.is_open()) {
        string theText;
        while (getline(filePointer_ver, theText)) {
            vertexes.append(new Vertex(theText));
        }
        updateDropdowns();
    }
    filePointer_ver.close();

    // read edges
    filePointer_edg.open("assets/edges.csv", ios::in);
    if (filePointer_edg.is_open()) {
        string theText;
        while (getline(filePointer_edg, theText)) {
            stringstream line(theText);
            string current;
            ArrayList<int> data;
            while (getline(line, current, ',')) {
                data.append(stoi(current));
            }
            edges.append(new edgeInfo(data[0], data[1], data[2], data[3]));
        }
    }
    filePointer_edg.close();

    // add all verticies
    for (int i = 0; i < vertexes.size(); i++) {
        g.addVertex(vertexes[i]);
    }

    // add all Edges
    for (int i = 0; i < edges.size(); i++) {
        edgeInfo *edge = edges[i];

        g.addDirectedEdge(vertexes[edge->vertex1], vertexes[edge->vertex2], edge->distance, edge->cost);      
        g.addDirectedEdge(vertexes[edge->vertex2], vertexes[edge->vertex1], edge->distance, edge->cost);
    }

    startNode = vertexes[0];
    destNode = vertexes[0];
    prefNode = CHEAPEST;
}