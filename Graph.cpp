//
// Created by Eden Bar on 31/03/2023.
//

#include "Graph.h"
#include <iostream>
using namespace std;

vector<int> mergeInnerPaths(vector<int> path, vector<int> &innerPath, int pathIdx)
{
    vector<int> newPath;

    // Push path[0...idx]
    for (int i = 0; i < pathIdx; i++)
    {
        newPath.push_back(path[i]);
    }

    // Push inner path
    for (int i = 0; i < innerPath.size(); i++)
    {
        newPath.push_back(innerPath[i]);
    }

    // Push [idx+1...]
    for (int i = pathIdx + 1; i < path.size(); i++)
    {
        newPath.push_back(path[i]);
    }

    return newPath;
}

void printFormattedPath(vector<int> &path)
{
    cout << "(";
    for (int i = 0; i < path.size(); i++)
    {
        cout << path[i];
        if (i != path.size() - 1)
        {
            cout << ",";
        }
    }
    cout << ")" << endl;
}

Graph::Graph(int size, bool isDirected)
{
    this->size = size;
    this->numOfEdges = 0;
    this->isDirected = isDirected;
    adjacencyList.resize(size);
}

// Print the graph
void Graph::printGraph()
{
    for (int i = 1; i < size; i++)
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

    for (int i = 1; i < size; i++)
    {
        visited[i] = White;
    }

    for (int i = 1; i < size; i++)
    {
        if (visited[i] == White)
        {
            visit(i, visited);
        }
    }
}

void Graph::visit(int v, int *visited)
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

bool Graph::isAllVerticesBlack(int *visited)
{
    for (int i = 1; i < size; i++)
    {
        if (visited[i] != Black)
        {
            return false;
        }
    }
    return true;
}

/// Functions for DirectedGraph ///

// Add an edge to the directed graph at the end of the adjacency list as required
void DirectedGraph::addEdge(int src, int dest)
{
    adjacencyList[src].push_back(dest);
    numOfEdges++;
}

// Checks if directed graph is strongly connected
bool DirectedGraph::isStronglyConnected()
{
    int *visited = new int[size];

    DFS(*this, visited);

    if (!isAllVerticesBlack(visited))
    {
        delete[] visited;
        return false;
    }

    delete[] visited;

    int *visitedTranspose = new int[size];
    DirectedGraph *transposedGraph = buildTransposeGraph(this);

    DFS(*transposedGraph, visited);

    if (!isAllVerticesBlack(visited))
    {
        delete[] visitedTranspose;
        delete transposedGraph;

        return false;
    }

    delete[] visitedTranspose;
    delete transposedGraph;

    return true;
}

bool DirectedGraph::isEntryLevelAndExitLevelEqual()
{
    vector<int> entryLevel(size);
    vector<int> exitLevel(size);

    for (int i = 1; i < size; i++)
    {
        entryLevel[i] = 0;
        exitLevel[i] = 0;
    }

    for (int i = 1; i < size; i++)
    {
        for (int j : adjacencyList[i])
        {
            entryLevel[j]++;
            exitLevel[i]++;
        }
    }

    for (int i = 1; i < size; i++)
    {
        if (entryLevel[i] != exitLevel[i])
        {
            return false;
        }
    }

    return true;
}

DirectedGraph *DirectedGraph::buildTransposeGraph(DirectedGraph *g)
{
    DirectedGraph gClone = *g;
    DirectedGraph *transposedGraph = new DirectedGraph(gClone.size);

    int current = 1;

    while (current < transposedGraph->size)
    {
        if (gClone.adjacencyList[current].size() == 0)
        {
            current++;
        }
        else
        {
            int next = gClone.adjacencyList[current].front();
            gClone.adjacencyList[current].pop_front();
            transposedGraph->adjacencyList[next].push_back(current);
        }
    }

    return transposedGraph;
}

vector<int> DirectedGraph::findCircuit(int v, DirectedGraph *g)
{
    vector<int> circuit(0);
    int current = v;
    circuit.push_back(current);

    while (g->adjacencyList[current].size() != 0)
    {
        int neighborVertex = g->adjacencyList[current].front();
        g->adjacencyList[current].pop_front();
        g->numOfEdges--;
        circuit.push_back(neighborVertex);
        current = neighborVertex;
    }

    return circuit;
}

void DirectedGraph::isEulerian()
{
    if (!(isStronglyConnected() && isEntryLevelAndExitLevelEqual()))
    {
        cout << "The graph is not Eulerian" << endl;
    }

    cout << "The graph is Eulerian" << endl;
    printEulerianPath();
}

void DirectedGraph::printEulerianPath()
{
    DirectedGraph gClone = *this;
    int current = 1;
    int readIdx = 1; // Skipping first vertex, which is 1
    vector<int> path = findCircuit(current, &gClone);

    while (gClone.numOfEdges != 0)
    {
        current = path[readIdx];
        vector<int> innerPath = findCircuit(current, &gClone);
        path = mergeInnerPaths(path, innerPath, readIdx);
        readIdx++;
    }

    printFormattedPath(path);

    return;
}

/// Functions for UndirectedGraph ///

// Add an edge to the undirected graph at the end of the adjacency list as required
void UndirectedGraph::addEdge(int src, int dest)
{
    adjacencyList[src].push_back(dest);
    adjacencyList[dest].push_back(src);
    numOfEdges++;
}

bool UndirectedGraph::isConnected()
{
    int *visited = new int[size];
    bool isConnected = true;

    DFS(*this, visited);

    for (int i = 1; i < size && isConnected; i++)
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
    UndirectedGraph gClone = *this;
    vector<int> levels;
    levels.reserve(size);
    int numOfVerticesWithOddEdges = 0;

    for (int i = 1; i < size; i++)
    {
        levels[i] = 0;
    }

    for (int i = 1; i < size; i++)
    {
        while (!gClone.adjacencyList[i].empty())
        {
            int j = gClone.adjacencyList[i].front();
            levels[i]++;
            levels[j]++;
            gClone.adjacencyList[i].remove(j);
            gClone.adjacencyList[j].remove(i);
            gClone.numOfEdges--;
        }
    }

    for (int i = 1; i < size; i++)
    {
        if (levels[i] % 2 != 0)
        {
            return false;
        }
    }

    return true;
}

vector<int> UndirectedGraph::findCircuit(int v, UndirectedGraph *g)
{
    vector<int> circuit(0);
    int current = v;
    circuit.push_back(current);

    while (g->adjacencyList[current].size() != 0)
    {
        int neighborVertex = g->adjacencyList[current].front();
        g->adjacencyList[current].pop_front();
        g->adjacencyList[neighborVertex].remove(current);
        g->numOfEdges--;
        circuit.push_back(neighborVertex);
        current = neighborVertex;
    }

    return circuit;
}

void UndirectedGraph::isEulerian()
{
    if (!(isConnected() && isAllLevelsOfEdgesEven()))
    {
        cout << "The graph is not Eulerian" << endl;
        return;
    }

    cout << "The graph is Eulerian" << endl;
    printEulerianPath();
}

void UndirectedGraph::printEulerianPath()
{
    UndirectedGraph gClone = *this;
    int current = 1;
    int readIdx = 1; // Skipping first vertex, which is 1
    vector<int> path = findCircuit(current, &gClone);

    while (gClone.numOfEdges != 0)
    {
        current = path[readIdx];
        vector<int> innerPath = findCircuit(current, &gClone);
        path = mergeInnerPaths(path, innerPath, readIdx);
        readIdx++;
    }

    printFormattedPath(path);

    return;
}
