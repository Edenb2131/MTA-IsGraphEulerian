//
// Created by Eden Bar on 31/03/2023.
//

#include "Graph.h"
#include <iostream>
using namespace std;

Graph::Graph(int size, bool isDirected)
{
    this->size = size;
    this->isDirected = isDirected;
    adjacencyList.resize(size);
}

// Print the graph
void Graph::printGraph()
{
    for (int i = 0; i < size; i++)
    {
        cout << i << ": ";
        for (int j : adjacencyList[i])
        {
            cout << j << " ";
        }
        cout << endl;
    }
}

void Graph::DFS(Graph &g, int *visited)
{
    int size = g.getSize();

    for (int i = 0; i < size; i++)
    {
        visited[i] = White;
    }
    for (int i = 0; i < size; i++)
    {
        if (visited[i] == White)
        {
            visit(i, visited);
        }
    }
}

void Graph::visit(int v, int visited[])
{
    visited[v] = Gray;
    for (int i : adjacencyList[v])
    {
        if (visited[i] == White)
        {
            visit(i, visited); // (v, i) is a tree edge
        }
    }
    visited[v] = Black;
}

/// Functions for DirectedGraph ///

// Add an edge to the directed graph at the end of the adjacency list as required
void DirectedGraph::addEdge(int src, int dest)
{
    adjacencyList[src].push_back(dest);
}

// Checks if directed graph is strongly connected
bool DirectedGraph::isStrongConnected(const int *visited)
{
    for (int i = 0; i < size; i++)
    {
        if (visited[i] != Black)
        {
            return false;
        }
    }
    return true;
}

bool DirectedGraph::isEntryLevelAndExitLevelEqual()
{
    int *entryLevel = new int[size];
    int *exitLevel = new int[size];
    for (int i = 0; i < size; i++)
    {
        entryLevel[i] = 0;
        exitLevel[i] = 0;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j : adjacencyList[i])
        {
            entryLevel[j]++;
            exitLevel[i]++;
        }
    }
    for (int i = 0; i < size; i++)
    {
        if (entryLevel[i] != exitLevel[i])
        {
            return false;
        }
    }

    delete[] entryLevel;
    delete[] exitLevel;

    return true;
}

void DirectedGraph::isEulerian()
{
    int *visited = new int[size];
    DFS(*this, visited);

    if (isStrongConnected(visited) && isEntryLevelAndExitLevelEqual())
    {
        cout << "The graph is Eulerian" << endl;
    }
    else
    {
        cout << "The graph is not Eulerian" << endl;
    }

    delete[] visited;
}

void DirectedGraph::printEulerianPath()
{
    list<int> path;
    int current = 1;
    path.push_back(current);

    while (!path.empty())
    {
        if (adjacencyList[current].size() == 0)
        {
            cout << path.back() << " ";
            path.pop_back();
            if (!path.empty())
                current = path.back();
        }
        else
        {
            int next = adjacencyList[current].front();
            adjacencyList[current].pop_front();
            adjacencyList[next].remove(current);
            path.push_back(next);
            current = next;
        }
    }

    cout << endl;
}

/// Functions for UndirectedGraph ///

// Add an edge to the undirected graph at the end of the adjacency list as required
void UndirectedGraph::addEdge(int src, int dest)
{
    adjacencyList[src].push_back(dest);
    adjacencyList[dest].push_back(src);
}

bool UndirectedGraph::isConnected()
{
    int *visited = new int[size];
    bool isConnected = true;

    DFS(*this, visited);

    for (int i = 0; i < size && isConnected; i++)
    {
        if (visited[i] != Black)
        {
            isConnected = false;
        }
    }

    delete[] visited;

    return isConnected;
}

bool UndirectedGraph::isAllLevelsOfEdgesEven()
{
    int *levels = new int[size];
    for (int i = 0; i < size; i++)
    {
        levels[i] = 0;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j : adjacencyList[i])
        {
            levels[i]++;
            levels[j]++;
        }
    }
    for (int i = 0; i < size; i++)
    {
        if (levels[i] % 2 != 0)
        {
            return false;
        }
    }
    delete[] levels;
    return true;
}

void UndirectedGraph::isEulerian()
{
    if (isConnected() && isAllLevelsOfEdgesEven())
    {
        cout << "The graph is Eulerian" << endl;
        printEulerianPath();
    }
    else
    {
        cout << "The graph is not Eulerian" << endl;
    }
}

void UndirectedGraph::printEulerianPath()
{
    list<int> path;
    int current = 1;
    path.push_back(current);

    while (!path.empty())
    {
        if (adjacencyList[current].size() == 0)
        {
            cout << path.back() << " ";
            path.pop_back();
            if (!path.empty())
                current = path.back();
        }
        else
        {
            int next = adjacencyList[current].front();
            adjacencyList[current].pop_front();
            adjacencyList[next].remove(current);
            path.push_back(next);
            current = next;
        }
    }

    cout << endl;
}
