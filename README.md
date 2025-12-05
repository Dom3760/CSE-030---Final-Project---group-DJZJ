1. Simple Navigation
1 Description
In this project your task is to implement a program that allows users to plan for air travel between different airports. You can use the airports and routes defined in the support files that come with this project, or you can modify them to add more interesting routes.

The airports and routes are represented as a graph, where the vertices are airports, and the edges are direct flights from one airport to another. Please do not represent your airports and routes as a complete graph, there have to be some pairs of airports that are not directly connected by a flight.

A user should be able to select a starting point and a destination point, as well as a preference between 3 options. These are:

Cheapest price
Shortest travel time
Least number of stops
For simplicity we can assume that layovers take 0 units of time. The output we expect is a flight itinerary from the starting airport to the destination airport, with all the stops along the way, and the total cost of the flight, the time taken, and (trivially) the number of stops.

There should be examples in the data where a direct flight (or flight with fewer stops) is more expensive than a sequence of alternative flights. Based on the user preference, the program should output the correct route.

2 Requirements
You can implement this as a Bobcat UI application or as a command line application.
You must use the Graph data structure included in the support files of this project, which is the one presented in class.
The graph data should be loaded from text files that are easily modifiable.
You need to work in teams of 2 - 5 people.
3 Bells and Whistles
Visual representation of the graph
Fix memory leaks in the Graph code provided
