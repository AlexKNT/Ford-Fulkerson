#include <iostream>
#include <vector>
#include <queue>
#include <limits>

/**
 * @brief Represents a directed edge in the graph.
 */
struct Edge {
    int from, to, capacity, flow;
};

/**
 * @brief Represents the graph using an adjacency list.
 */
class Graph {
public:
    /**
     * @brief Constructs a graph with a given number of vertices.
     * @param numVertices The number of vertices in the graph.
     */
    explicit Graph(int numVertices);

    /**
     * @brief Adds a directed edge to the graph.
     * @param from The source vertex.
     * @param to The destination vertex.
     * @param capacity The capacity of the edge.
     */
    void addEdge(int from, int to, int capacity);

    /**
     * @brief Finds the maximum flow in the graph using the Ford-Fulkerson algorithm.
     * @param source The source vertex.
     * @param sink The sink vertex.
     * @return The maximum flow in the graph.
     */
    int fordFulkerson(int source, int sink);

private:
    int numVertices;
    std::vector<std::vector<int>> residualCapacity;
    std::vector<std::vector<int>> graph;
    std::vector<Edge> edges;

    /**
     * @brief Initializes the residual capacity of the graph.
     */
    void initializeResidualCapacity();

    /**
     * @brief Uses Breadth-First Search (BFS) to find an augmenting path in the residual graph.
     * @param parent The array to store the parent of each vertex in the augmenting path.
     * @param sink The sink vertex.
     * @return True if an augmenting path is found, false otherwise.
     */
    bool bfs(std::vector<int>& parent, int sink);

    /**
     * @brief Updates the residual graph by augmenting the flow along the augmenting path.
     * @param source The source vertex.
     * @param sink The sink vertex.
     * @param parent The array containing the parent of each vertex in the augmenting path.
     * @return The bottleneck capacity of the augmenting path.
     */
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

    // Continue finding augmenting paths until no more paths can be found
    std::vector<int> parent(numVertices);
    while (bfs(parent, sink)) {
        int bottleneck = updateResidualGraph(source, sink, parent);
        if (bottleneck == 0) {
            break;  // No augmenting path found
        }
        maxFlow += bottleneck;
    }

    return maxFlow;
}

void Graph::initializeResidualCapacity() {
    residualCapacity.assign(numVertices, std::vector<int>(numVertices, 0));

    // Initialize residual capacity based on the original graph
    for (const Edge& edge : edges) {
        residualCapacity[edge.from][edge.to] = edge.capacity;
    }
}

bool Graph::bfs(std::vector<int>& parent, int sink) {
    parent.assign(numVertices, -1);
    std::queue<int> q;
    q.push(0); // Start BFS from the source vertex

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (int i : graph[current]) {
            const Edge& edge = edges[i];
            if (edge.to == sink && residualCapacity[edge.from][edge.to] > 0) {
                parent[edge.to] = edge.from;
                return true;  // Stop BFS if the sink vertex is reached
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

    // Find the bottleneck capacity along the augmenting path
    for (int current = sink; current != source; current = parent[current]) {
        int parentIdx = parent[current];
        bottleneck = std::min(bottleneck, residualCapacity[parentIdx][current]);
    }

    // Update the residual capacities of the edges along the augmenting path
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

    std::cout << "Enter edges in the format (from to capacity) (example: 0 1 2. edge goes from vertex 0 to vertex 1 and has value of 2) starts from 0:\n";
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
