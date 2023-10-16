#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <limits>

// The Ford-Fulkerson algorithm is a method for finding the maximum flow in a flow network, 
// which is a directed graph representing the flow of resources from a source node to a sink node through a network of interconnected nodes.

/* 
    Here's a step-by-step explanation of how it works:

    Initialization: Start with an initial flow of zero on all edges.

    Find Augmenting Paths: While there exists a path from the source to the sink in the residual graph (a modified version of the original graph), 
    find an augmenting path. 
    An augmenting path is a path from the source to the sink through edges with available capacity (capacity minus flow is greater than zero).

    Find Bottleneck Capacity: On the augmenting path, find the edge with the minimum available capacity. This capacity is called the bottleneck capacity.

    Update Flow: Increase the flow on each edge of the augmenting path by the bottleneck capacity. Decrease the flow on the reverse edges (if the graph is directed) by the same amount.

    Repeat: Go back to step 2 and repeat the process until no more augmenting paths can be found.

    Termination: When there are no more augmenting paths, the algorithm terminates, and the flow obtained at that point is the maximum flow in the network.
*/


constexpr int INF = std::numeric_limits<int>::max();

// maximum number of vertices (nodes) in the flow network
constexpr int MAX_V = 100;

struct Edge {
    int to, capacity, flow;
    // This is the constructor of the Edge struct. 
    // It allows you to create an Edge object and initialize its to, capacity, and flow members with the provided values. 
    // The constructor sets the flow to 0 by default.
    Edge(int t, int c) : to(t), capacity(c), flow(0) {}
};

std::vector<Edge> graph[MAX_V];
bool visited[MAX_V];

void addEdge(int from, int to, int capacity) {
    graph[from].emplace_back(to, capacity);
    // Add a backward edge with capacity 0 for directed graphs
    graph[to].emplace_back(from, 0);
}

bool findAugmentingPath(int source, int sink) {
    std::memset(visited, 0, sizeof(visited)); // Reset the visited array

    std::queue<int> q;
    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (Edge& edge : graph[u]) {
            int v = edge.to;
            // Check if the edge is not visited and has remaining capacity
            if (visited[v] || edge.capacity - edge.flow <= 0) {
                continue; // Skip this edge if it's not eligible
            }

            q.push(v); // Add the node to the queue
            visited[v] = true; // Mark the node as visited
        }
    }

    // Returns true if an augmenting path from source to sink is found
    return visited[sink];
}


int getMinCapacity(int source, int sink) {
    int minCapacity = INF;
    for (int v = sink; v != source; v = graph[v][0].to) {
        for (Edge& edge : graph[v]) {
            if (visited[edge.to]) {
                // Find the minimum capacity along the path
                minCapacity = std::min(minCapacity, edge.capacity - edge.flow);
                break; // Break to avoid unnecessary iterations
            }
        }
    }
    return minCapacity;
}

void updateFlow(int source, int sink, int minCapacity) {
    for (int v = sink; v != source; v = graph[v][0].to) {
        for (Edge& edge : graph[v]) {
            if (visited[edge.to]) {
                edge.flow += minCapacity; // Increase the forward flow
                for (Edge& backEdge : graph[edge.to]) {
                    if (backEdge.to == v) {
                        backEdge.flow -= minCapacity; // Decrease the backward flow
                        break; // Break to avoid unnecessary iterations
                    }
                }
                break; // Break to avoid unnecessary iterations
            }
        }
    }
}

int fordFulkerson(int source, int sink) {
    int maxFlow = 0; // Initialize the maximum flow to 0

    while (findAugmentingPath(source, sink)) {
        int minCapacity = getMinCapacity(source, sink);
        updateFlow(source, sink, minCapacity);
        maxFlow += minCapacity; // Update the maximum flow with the path's capacity
    }

    return maxFlow; // Return the maximum flow
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