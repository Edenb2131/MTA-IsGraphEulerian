#include "Graph.h"
#include <exception>
#include <iostream>

using namespace std;

// Getting input from user regarding the setting of the graph
void getInputFromUser(char &isDirected, int &numOfVertices, int &numOfEdges)
{

    cout << "Is the graph directed? (y/n)" << endl;
    cin >> isDirected;

    // Check if the input is valid
    if (isDirected != 'y' && isDirected != 'n' && isDirected != 'Y' && isDirected != 'N')
        throw exception();

    cout << "Please enter the number of vertices in the graph:" << endl;
    cin >> numOfVertices;

    cout << "Please enter the number of edges in the graph:" << endl;
    cin >> numOfEdges;

    if (numOfVertices <= 0 || numOfEdges <= 0)
    {
        throw exception();
    }
    // We change number of vertices to start from 1 instead of 0
    numOfVertices++;
}

void getEdgesFromUser(int numOfEdges, int numOfVertices, Graph *graph)
{

    int src, dest;
    for (int i = 0; i < numOfEdges; i++)
    {
        // cout << "Please enter the source and destination of edge " << i << ":" << endl;
        cin >> src >> dest;

        // Check if the input is valid
        if (src < 1 || src >= numOfVertices || dest < 0 || dest >= numOfVertices)
            throw exception();

        graph->addEdge(src, dest);
    }
}

int main()
{

    char isDirected;
    int numOfVertices; // This will be the required n
    int numOfEdges;    // This will be the required m

    try
    {
        getInputFromUser(isDirected, numOfVertices, numOfEdges);
    }
    catch (exception &e)
    {
        cout << "invalid input" << endl;
        exit(1);
    }

    if (isDirected == 'y' || isDirected == 'Y')
    {
        DirectedGraph graph(numOfVertices); // Create a directed graph

        try
        {
            getEdgesFromUser(numOfEdges, numOfVertices, &graph);
        }
        catch (exception &e)
        {
            cout << "invalid input" << endl;
            exit(1);
        }

        graph.printGraph();

        graph.isEulerian();
    }
    else
    {
        UndirectedGraph graph(numOfVertices); // Create an undirected graph

        try
        {
            getEdgesFromUser(numOfEdges, numOfVertices, &graph);
        }
        catch (exception &e)
        {
            cout << "invalid input" << endl;
            exit(1);
        }

        graph.printGraph();

        graph.isEulerian();
    }

    return 0;
}
