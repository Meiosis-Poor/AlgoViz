//
// Created by poorc on 2025/9/29.
//

#ifndef SIMULATOR_GRAPHWORKER_H
#define SIMULATOR_GRAPHWORKER_H

#include <QPair>
#include <QList>
#include <QQueue>
#include <QVector>
#include <QStack>

struct Step {
    enum Type { VisitNode, TraverseEdge, UpdateDist } type;
    int node;
    QPair<int,int> edge;
};

class GraphWorker {
public:
    GraphWorker();
    //GraphWorker(const QVector<QVector<QPair<int,int>>>& adjList);
    void setData(const QVector<QVector<int>>& myintdata);
    void setData(const QVector<QVector<QPair<int,int>>>& myadjList);
    void init();
    void BFS(int start);
    void DFS(int u);
    void BFSall();
    void DFSall();
    void prim(int start=0);
    void primall();
    void kruskal();
    QVector<int> dijkstra(int start);
    Step nextStep();

private:
    int n;
    bool useMatrix;
    QVector<QVector<int>> intdata;
    QVector<QVector<QPair<int,int>>> pairdata;
    QVector<bool> visited;
    QQueue<int> bfsQueue;
    QVector<int> dist;
    QQueue<Step> stepBuffer;

};


#endif //SIMULATOR_GRAPHWORKER_H
