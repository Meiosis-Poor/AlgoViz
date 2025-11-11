#include "graphworker.h"
#include <queue>
#include <QSet>


GraphWorker::GraphWorker(){}

void GraphWorker::setData(const QVector<QVector<int>> &myintdata) {
    intdata.clear();
    pairdata.clear();
    n = myintdata.size();
    useMatrix = true;
    intdata = myintdata;
}

void GraphWorker::setData(const QVector<QVector<QPair<int, int>>> &myadjList) {
    intdata.clear();
    pairdata.clear();
    n = myadjList.size();
    useMatrix = false;
    pairdata = myadjList;
}


void GraphWorker::init() {
    stepBuffer.clear();
    visited.fill(false,n);
}

void GraphWorker::BFS(int start) {
    QQueue<int> q;
    q.enqueue(start);
    visited[start] = true;
    stepBuffer.push_back({Step::VisitNode, start, {}});

    while(!q.isEmpty()) {
        int u = q.dequeue();
        if(useMatrix) {
            for(int v=0; v<n; v++) {
                if(intdata[u][v] && !visited[v]) {
                    visited[v] = true;
                    q.enqueue(v);
                    stepBuffer.push_back({Step::TraverseEdge, -1, {u,v}});
                    stepBuffer.push_back({Step::VisitNode, v, {}});
                }
            }
        } else {
            for(auto &e : pairdata[u]) {
                int v = e.first;
                if(!visited[v]) {
                    visited[v] = true;
                    q.enqueue(v);
                    stepBuffer.push_back({Step::TraverseEdge, -1, {u,v}});
                    stepBuffer.push_back({Step::VisitNode, v, {}});
                }
            }
        }
    }
}

void GraphWorker::BFSall() {
    init();
    for(int u=0; u<n; u++) {
        if(!visited[u]) {
            BFS(u);
        }
    }
}

void GraphWorker::DFS(int u) {
    visited[u] = true;
    stepBuffer.push_back({Step::VisitNode, u, {}});
    if(useMatrix) {
        for(int v=0; v<n; v++) {
            if(intdata[u][v] && !visited[v]) {
                stepBuffer.push_back({Step::TraverseEdge, -1, {u,v}});
                DFS(v);
            }
        }
    }
    else {
        for(auto &e : pairdata[u]) {
            int v = e.first;
            if(!visited[v]) {
                stepBuffer.push_back({Step::TraverseEdge, -1, {u,v}});
                DFS(v);
            }
        }
    }
}

void GraphWorker::DFSall() {
    init();
    for(int u=0; u<n; u++) {
        if(!visited[u]) {
            DFS(u);
        }
    }
}

void GraphWorker::prim(int start) {
    std::priority_queue<QPair<int,int>, std::vector<QPair<int,int>>, std::greater<QPair<int,int>>> pq;
    pq.push({0,start});
    visited[start] = true;
    stepBuffer.push_back({Step::VisitNode, start, {}});

    while(!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (useMatrix) {
            for (int v = 0; v < n; v++) {
                if (intdata[u][v] && !visited[v]) {
                    pq.push({intdata[u][v], v});
                    visited[v] = true;
                    stepBuffer.push_back({Step::TraverseEdge, -1, {u, v}});
                    stepBuffer.push_back({Step::VisitNode, v, {}});
                }
            }
        }
        else {
            for (auto &e: pairdata[u]) {
                int v = e.first;
                if (!visited[v]) {
                    pq.push({e.second, v});
                    visited[v] = true;
                    stepBuffer.push_back({Step::TraverseEdge, -1, {u, v}});
                    stepBuffer.push_back({Step::VisitNode, v, {}});
                }
            }
        }
    }
}

void GraphWorker::primall() {
    init();
    for(int u=0; u<n; u++) {
        if(!visited[u]) {
            prim(u);
        }
    }
}

void GraphWorker::kruskal() {
    init();
    QVector<QPair<int,QPair<int,int>>> edges;
    if(useMatrix) {
        for(int u=0; u<n; u++) {
            for(int v=0; v<n; v++) {
                if(intdata[u][v]) {
                    edges.push_back({intdata[u][v], {u,v}});
                }
            }
        }
    } else {
        for(int u=0; u<n; u++) {
            for(auto &e : pairdata[u]) {
                edges.push_back({e.second, {u,e.first}});
            }
        }
    }
    std::sort(edges.begin(), edges.end(),[](QPair<int,QPair<int,int>> a, QPair<int,QPair<int,int>> b){
        return a.first < b.first;
    });
    QVector<int> parent(n);
    for(int i=0; i<n; i++) {
        parent[i] = i;
    }
    std::function<int(int)> find = [&](int u) -> int {
        if(parent[u] == u) return u;
        return find(parent[u]);
    };
    for(auto &e : edges) {
        int u = e.second.first;
        int v = e.second.second;
        int pu = find(u);
        int pv = find(v);
        if(pu != pv) {
            parent[pu] = pv;
            if(!visited[u]) {
                visited[u] = true;
                stepBuffer.push_back({Step::VisitNode, u, {}});
            }
            if(!visited[v]) {
                visited[v] = true;
                stepBuffer.push_back({Step::VisitNode, v, {}});
            }
            stepBuffer.push_back({Step::TraverseEdge, -1, {u,v}});
        }
    }
}

QVector<int> GraphWorker::dijkstra(int start) {
    init();
    dist.fill(INT_MAX, n);
    dist[start] = 0;
    QVector<int> fa(n);
    using P = QPair<int,int>; // <distance, node>

    // priority_queue 默认大顶堆，所以使用负距离
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, start});

    while(!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if(visited[u]) continue;
        visited[u] = true;
        stepBuffer.push_back({Step::VisitNode, u, {-1,-1}}); // 确定最短路

        if(useMatrix) {
            for(int v=0; v<n; v++) {
                if(intdata[u][v] != 0 && !visited[v]) {
                    int w = intdata[u][v];
                    stepBuffer.push_back({Step::TraverseEdge, -1, {u,v}}); // 松弛边
                    if(dist[u] + w < dist[v]) {
                        int temp = dist[v];
                        dist[v] = dist[u] + w;
                        pq.push({dist[v], v});
                        fa[v]=u;
                        stepBuffer.push_back({Step::UpdateDist, v, {temp,dist[v]}});
                    }
                }
            }
        } else {
            for(auto &e : pairdata[u]) {
                int v = e.first;
                int w = e.second;
                if(!visited[v]) {
                    stepBuffer.push_back({Step::TraverseEdge, -1, {u,v}}); // 松弛边
                    if(dist[u] + w < dist[v]) {
                        int temp = dist[v];
                        dist[v] = dist[u] + w;
                        pq.push({dist[v], v});
                        fa[v]=u;
                        stepBuffer.push_back({Step::UpdateDist, v, {temp,dist[v]}});
                    }
                }
            }
        }
    }
    return fa;
}



Step GraphWorker::nextStep() {
    if(stepBuffer.isEmpty()) return {Step::VisitNode, -1, {-1,-1}};
    Step step = stepBuffer.front();
    stepBuffer.pop_front();
    return step;
}