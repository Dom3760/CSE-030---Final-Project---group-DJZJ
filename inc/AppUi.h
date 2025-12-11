#ifndef AppUi_H
#define AppUi_H

#include "ArrayList.h"
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/button.h>
#include <bobcat_ui/dropdown.h>
#include <bobcat_ui/window.h>
#include <FL/Fl_Scroll.H>
#include <Graph.h>

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

class AppUi : public bobcat::Application_ {
    bobcat::Window *window;
    bobcat::Dropdown *start;
    bobcat::Dropdown *destination;
    bobcat::Button *search;
    bobcat::Button *clear;
    Fl_Scroll *results;

    ArrayList<Vertex*> vertexes;
    ArrayList<edgeInfo*> edges;
    Graph g;

    void init();
    void onClick(bobcat::Widget *sender);
    void onClear(bobcat::Widget *sender);
public:
    AppUi();
};

#endif