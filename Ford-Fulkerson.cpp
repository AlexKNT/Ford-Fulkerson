#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

constexpr int INF = 1e9;
constexpr int MAX_V = 100;

struct Edge {
    int to, capacity, flow;
    Edge(int t, int c) : to(t), capacity(c), flow(0) {}
};

std::vector<Edge> graph[MAX_V];
bool visited[MAX_V];

void addEdge(int from, int to, int capacity) {
    graph[from].emplace_back(to, capacity);
    // Add a backward edge with capacity 0 for directed graphs
    graph[to].emplace_back(from, 0);
}

int fordFulkerson(int source, int sink) {
    int maxFlow = 0;

    while (true) {
        std::memset(visited, 0, sizeof(visited));
        std::queue<int> q;
        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (Edge& edge : graph[u]) {
                int v = edge.to;
                if (!visited[v] && edge.capacity - edge.flow > 0) {
                    q.push(v);
                    visited[v] = true;
                }
            }
        }

        if (!visited[sink]) {
            break;
        }

        int minCapacity = INF;
        for (int v = sink; v != source; v = graph[v][0].to) {
            for (Edge& edge : graph[v]) {
                if (visited[edge.to]) {
                    minCapacity = std::min(minCapacity, edge.capacity - edge.flow);
                    break;
                }
            }
        }

        for (int v = sink; v != source; v = graph[v][0].to) {
            for (Edge& edge : graph[v]) {
                if (visited[edge.to]) {
                    edge.flow += minCapacity;
                    for (Edge& backEdge : graph[edge.to]) {
                        if (backEdge.to == v) {
                            backEdge.flow -= minCapacity;
                            break;
                        }
                    }
                    break;
                }
            }
        }

        maxFlow += minCapacity;
    }

    return maxFlow;
}

int main() {
    int source = 0;
    int sink = 5;

    addEdge(0, 1, 10);
    addEdge(0, 2, 5);
    addEdge(1, 2, 15);
    addEdge(1, 3, 5);
    addEdge(2, 3, 10);
    addEdge(2, 4, 10);
    addEdge(3, 4, 5);
    addEdge(4, 5, 10);

    int maxFlow = fordFulkerson(source, sink);
    std::cout << "Maximum Flow: " << maxFlow << '\n';

    return 0;
}
