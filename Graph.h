//
// Created by Eden Bar on 31/03/2023.
//

#ifndef ALGO_EX1_GRAPH_H
#define ALGO_EX1_GRAPH_H

#define White 0
#define Gray 1
#define Black 2

#include <list>
#include <vector>

class Graph
{
    friend class DirectedGraph;
    friend class UndirectedGraph;

  protected:
    std::vector<std::list<int>> adjacencyList;
    bool isDirected;
    int size; // numOfVertices
    int numOfEdges;

  public:
    Graph(int size, bool isDirected);
    void printGraph();
    int getSize() const
    {
        return size;
    }
    int getNumOfEdges() const
    {
        return numOfEdges;
    }
    bool getIsDirected()
    {
        return isDirected;
    }
    void DFS(Graph &g, int *);
    void visit(int v, int *);
    bool isAllVerticesBlack(int *visited);
    virtual void addEdge(int src, int dest) = 0;
    virtual void isEulerian() = 0;
    virtual void printEulerianPath() = 0;

    // destructor
};

class DirectedGraph : public Graph
{
  public:
    explicit DirectedGraph(int size) : Graph(size, true)
    {
    }
    void addEdge(int src, int dest) override;
    bool isStronglyConnected();
    bool isEntryLevelAndExitLevelEqual();
    DirectedGraph *buildTransposeGraph(DirectedGraph *g);
    std::vector<int> findCircuit(int v, DirectedGraph *g);
    void isEulerian() override;
    void printEulerianPath() override;
};

class UndirectedGraph : public Graph
{
  public:
    explicit UndirectedGraph(int size) : Graph(size, false)
    {
    }
    void addEdge(int src, int dest) override;
    bool isConnected();
    bool isAllLevelsOfEdgesEven();
    std::vector<int> findCircuit(int v, UndirectedGraph *g);
    void isEulerian() override;
    void printEulerianPath() override;
};

#endif // ALGO_EX1_GRAPH_H
