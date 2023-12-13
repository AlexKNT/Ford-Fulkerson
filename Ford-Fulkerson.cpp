#include <iostream>
#include <vector>
#include <queue>
#include <limits>

struct Edge {
    int from, to, capacity, flow;
};

class Graph {
public:
    explicit Graph(int numVertices);

    void addEdge(int from, int to, int capacity);

    int fordFulkerson(int source, int sink);

private:
    int numVertices;
    std::vector<std::vector<int>> residualCapacity;
    std::vector<std::vector<int>> graph;
    std::vector<Edge> edges;

    void initializeResidualCapacity();

    bool bfs(std::vector<int>& parent, int sink);

    int updateResidualGraph(int source, int sink, const std::vector<int>& parent);
};

Graph::Graph(int numVertices) : numVertices(numVertices) {
    graph.resize(numVertices);
}

void Graph::addEdge(int from, int to, int capacity) {
    graph[from].push_back(edges.size());
    edges.push_back({ from, to, capacity, 0 });
}

int Graph::fordFulkerson(int source, int sink) {
    initializeResidualCapacity();

    int maxFlow = 0;
    int iteration = 1;

    std::vector<int> parent(numVertices);
    while (bfs(parent, sink)) {
        int bottleneck = updateResidualGraph(source, sink, parent);
        if (bottleneck == 0) {
            break;
        }

        std::cout << "Iteration " << iteration << ": ";
        std::cout << "Bottleneck Capacity = " << bottleneck << std::endl;

        ++iteration;
        maxFlow += bottleneck;
    }

    return maxFlow;
}

void Graph::initializeResidualCapacity() {
    residualCapacity.assign(numVertices, std::vector<int>(numVertices, 0));

    for (const Edge& edge : edges) {
        residualCapacity[edge.from][edge.to] = edge.capacity;
    }
}

bool Graph::bfs(std::vector<int>& parent, int sink) {
    parent.assign(numVertices, -1);
    std::queue<int> q;
    q.push(0);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int i : graph[current]) {
            const Edge& edge = edges[i];
            if (edge.to == sink && residualCapacity[edge.from][edge.to] > 0) {
                parent[edge.to] = edge.from;
                return true;
            }
            if (parent[edge.to] == -1 && residualCapacity[edge.from][edge.to] > 0) {
                parent[edge.to] = edge.from;
                q.push(edge.to);
            }
        }
    }

    return false;
}

int Graph::updateResidualGraph(int source, int sink, const std::vector<int>& parent) {
    int bottleneck = std::numeric_limits<int>::max();

    for (int current = sink; current != source; current = parent[current]) {
        int parentIdx = parent[current];
        bottleneck = std::min(bottleneck, residualCapacity[parentIdx][current]);
    }

    for (int current = sink; current != source; current = parent[current]) {
        int parentIdx = parent[current];
        residualCapacity[parentIdx][current] -= bottleneck;
        residualCapacity[current][parentIdx] += bottleneck;
    }

    return bottleneck;
}

int main() {
    int numVertices, numEdges;
    std::cout << "Enter the number of vertices: ";
    std::cin >> numVertices;

    std::cout << "Enter the number of edges: ";
    std::cin >> numEdges;

    Graph graph(numVertices);

    std::cout << "Enter edges in the format (from to capacity): ";
    std::cout << "(example: 0 1 2. edge goes from vertex 0 to vertex 1 and has a capacity of 2)\n";

    for (int i = 0; i < numEdges; ++i) {
        int from, to, capacity;
        std::cin >> from >> to >> capacity;
        graph.addEdge(from, to, capacity);
    }

    int source, sink;
    std::cout << "Enter the source vertex: ";
    std::cin >> source;

    std::cout << "Enter the sink vertex: ";
    std::cin >> sink;

    int maxFlow = graph.fordFulkerson(source, sink);

    std::cout << "Maximum flow from vertex " << source << " to vertex " << sink << " is: " << maxFlow << std::endl;

    return 0;
}
