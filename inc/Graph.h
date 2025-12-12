#ifndef GRAPH_H
#define GRAPH_H

#include "LinkedList.h"
#include <ArrayList.h>
#include <Enums.h>
#include <HashTable.h>
#include <Queue.h>
#include <Stack.h>
#include <cstddef>
#include <ostream>
#include <string>

struct Edge;

struct Vertex {
    std::string data;
    ArrayList<Edge *> edgeList;

    Vertex(std::string data) { this->data = data; }
};

inline std::ostream &operator<<(std::ostream &os, Vertex *v) {
    os << v->data;

    return os;
}

// changed Edge struct to have integers for time and cost and took out weight
struct Edge {
    Vertex *from;
    Vertex *to;
    // int weight;
    int time;
    int cost;

    Edge(Vertex *from, Vertex *to, int t, int c) {
        this->from = from;
        this->to = to;
        // this->weight = w;
        this->time = t;
        this->cost = c;
    }
};

inline std::ostream &operator<<(std::ostream &os, Edge *e) {
    os << "(" << e->from << ", " << e->to << ") - " << e->time << " - "
       << e->cost;

    return os;
}
// changed Waypoint struct to have integers for total time, cost, and stops and
// took out weight
struct Waypoint {
    Waypoint *parent;
    Vertex *vertex;
    ArrayList<Waypoint *> children;
    int partialCost;
    // int weight;

    int totalTime;
    int totalCost;
    int totalStops;

    Waypoint(Vertex *v) {
        this->parent = nullptr;
        this->vertex = v;
        this->partialCost = 0;

        this->totalTime = 0;
        this->totalCost = 0;
        this->totalStops = 0;
    }

    // commented out because this logic will be put inside the main search
    // function
    //  void expand() {
    //      for (int i = 0; i < vertex->edgeList.size(); i++) {
    //          Waypoint *temp = new Waypoint(vertex->edgeList[i]->to);
    //          temp->parent = this;
    //          temp->weight = vertex->edgeList[i]->weight;
    //          temp->partialCost = partialCost + vertex->edgeList[i]->weight;
    //          children.append(temp);
    //      }
    //  }
};

inline std::ostream &operator<<(std::ostream &os, Waypoint *wp) {
    std::string p = "null";
    if (wp->parent != nullptr) {
        p = wp->parent->vertex->data;
    }

    os << p << " -> " << wp->vertex->data;

    return os;
}

struct Graph {
    ArrayList<Vertex *> vertices;

    void addVertex(Vertex *v) { vertices.append(v); }

    void addEdge(Vertex *x, Vertex *y, int w) {
        // x->edgeList.append(new Edge(x, y, w));
        // y->edgeList.append(new Edge(y, x, w));
    }

    void addDirectedEdge(Vertex *x, Vertex *y, int t, int c) {
        // x->edgeList.append(new Edge(x, y, w));
        x->edgeList.append(new Edge(x, y, t, c));
    }

    Waypoint *bfs(Vertex *start, Vertex *destination) {
        std::cout << "Running Breadth-First Search" << std::endl;
        Queue<Waypoint *> frontier;
        HashTable<std::string> seen;

        Waypoint *first = new Waypoint(start);

        frontier.enqueue(first);
        seen.insert(first->vertex->data);

        Waypoint *result = nullptr;

        while (!frontier.isEmpty()) {
            result = frontier.dequeue();

            if (result->vertex == destination) {
                return result;
            }

            // result->expand();
            // Get the neighbors of the current vertex
            // that we are on...

            // The neighbors are stored in the result->children array
            std::cout << std::endl
                      << "Expanding " << result->vertex->data << std::endl;

            for (int i = 0; i < result->children.size(); i++) {
                // For every child of the result node
                // If we have not seen it
                // We add it to the frontier (as a queue)
                // We mark it as seen
                if (!seen.search(result->children[i]->vertex->data)) {
                    std::cout << "Adding " << result->children[i]->vertex->data
                              << std::endl;
                    frontier.enqueue(result->children[i]);
                    seen.insert(result->children[i]->vertex->data);
                }
            }

            std::cout << std::endl << "Frontier" << std::endl;

            Link<Waypoint *> *temp = frontier.list.front;
            while (temp != nullptr) {
                std::cout << "(" << temp->data->vertex->data << ","
                          << temp->data->partialCost << ")";
                temp = temp->next;

                if (temp != nullptr) {
                    std::cout << ", ";
                } else {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
        }

        return nullptr;
    }

    Waypoint *dfs(Vertex *start, Vertex *destination) {
        std::cout << "Running Depth-First Search" << std::endl;

        Stack<Waypoint *> frontier;
        HashTable<std::string> seen;

        Waypoint *first = new Waypoint(start);

        frontier.push(first);
        seen.insert(first->vertex->data);

        Waypoint *result = nullptr;

        while (!frontier.isEmpty()) {
            result = frontier.pop();

            if (result->vertex == destination) {
                return result;
            }

            // result->expand();

            std::cout << std::endl
                      << "Expanding " << result->vertex->data << std::endl;

            for (int i = 0; i < result->children.size(); i++) {
                if (!seen.search(result->children[i]->vertex->data)) {
                    std::cout << "Adding " << result->children[i]->vertex->data
                              << std::endl;
                    frontier.push(result->children[i]);
                    seen.insert(result->children[i]->vertex->data);
                }
            }

            std::cout << std::endl << "Frontier" << std::endl;

            Link<Waypoint *> *temp = frontier.front;
            while (temp != nullptr) {
                std::cout << "(" << temp->data->vertex->data << ","
                          << temp->data->partialCost << ")";
                temp = temp->next;

                if (temp != nullptr) {
                    std::cout << ", ";
                } else {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
        }

        return nullptr;
    }
    // renamed function to findOptimalPath and made it so that it accepts
    // "Optimization" Enum as parameter
    Waypoint *findOptimalPath(Vertex *start, Vertex *destination, Optimization preference) {
        std::cout << "Running Uniform Cost Search" << std::endl;

        // Should be a priority queue
        ArrayList<Waypoint *> frontier;
        HashTable<std::string> seen;

        Waypoint *first = new Waypoint(start);

        frontier.append(first);
        seen.insert(first->vertex->data);

        Waypoint *result = nullptr;

        // // my code
        // if (preference == CHEAPEST) {

        // } else if (preference == SHORTEST_TIME) {

        // } else if (preference == LEAST_STOPS) {
        // }

        while (frontier.size() != 0) {
            result = frontier.removeLast();

            if (result->vertex == destination) {
                return result;
            }

            //inserted for loop to go throught each edge among the vertexes. 
            for(int i = 0; i < result->vertex->edgeList.size(); i++)
            {
                // loop that goes through each vertext, looks at neighbors, and then changes edge weight based on preference
                Edge* currentEdge = result->vertex->edgeList[i];
                Vertex* neighbor = currentEdge->to;
                int edgeWeight;

                if (preference == CHEAPEST) {
                    edgeWeight = currentEdge->cost;
                }
                else if (preference == SHORTEST_TIME) {
                    edgeWeight= currentEdge->time;
                }
                else {
                    edgeWeight = 1;
                }

                int newPartialCost = result->partialCost + edgeWeight;

                // updating total variables with each pass through

                Waypoint* newWp = new Waypoint(neighbor);
                newWp->parent = result;
                newWp->partialCost = newPartialCost;
                
                newWp->totalTime = result->totalTime + currentEdge->time;
                newWp->totalCost = result->totalCost + currentEdge->cost;
                newWp->totalStops = result->totalStops + 1;

                // 
                if(!seen.search(newWp->vertex->data))
                {
                    frontier.append(newWp);
                    seen.insert(newWp->vertex->data);

                    //inital sorting algorithm
                    int j = frontier.size() - 1;
                    while (j > 0 && frontier.data[j]->partialCost > frontier.data[j - 1]->partialCost) {
                        Waypoint *temp = frontier.data[j];
                        frontier.data[j] = frontier.data[j - 1];
                        frontier.data[j - 1] = temp;
                        j--;
                    }
                }
                else
                {
                    Waypoint* worsePath = nullptr;
                    for (int k = 0; k < frontier.size(); k++)
                    {
                        if(frontier[k]->vertex->data == newWp->vertex->data)
                        {
                            if(frontier[k]->partialCost > newWp->partialCost)
                            {
                                worsePath = frontier[k];
                                break;
                            }
                        }
                    }
                    if(worsePath)
                    {   
                        //debugging statements
                        std::cout << "Found another way to get to "
                        << newWp->vertex->data << ". Was "
                        << worsePath->partialCost << ", but now it is "
                        << newWp->partialCost << std::endl;

                        //code snippets from below slightly modified
                        for (int k = 0; k < frontier.size(); k++) {
                            if (frontier[k]->parent == worsePath) {
                                frontier[k]->parent = newWp;
                            }
                        }

                        for (int k = 0; k < frontier.size(); k++) {
                            if (frontier[k] == worsePath) {
                                delete frontier[k];
                                frontier[k] = newWp;
                                break;
                            }
                        }

                        for (int a = 1; a < frontier.size(); a++) {
                            int b = a;
                            while (b > 0 && frontier[b]->partialCost > frontier[b - 1]->partialCost) {
                                Waypoint *x = frontier[b];
                                frontier[b] = frontier[b - 1];
                                frontier[b - 1] = x;
                                b--;
                            }
                        }
                    }
                    else
                    {
                        delete newWp;
                    }
                }
            }

            //debugging
            std::cout << std::endl << "Frontier" << std::endl;
            for (int k = frontier.size() - 1; k >= 0; k--) {
                std::cout << "(" << frontier[k]->vertex->data << ", "
                          << frontier[k]->partialCost << ") ";
                if (k > 0) {
                    std::cout << ", ";
                } else {
                    std::cout << std::endl;
                }
            }
            std::cout << std::endl;
        }
        return nullptr;
    }
};

inline std::ostream &operator<<(std::ostream &os, const Graph &g) {
    for (int i = 0; i < g.vertices.size(); i++) {
        os << g.vertices[i]->edgeList << std::endl;
    }

    return os;
}

#endif