#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <chrono>
#include <algorithm>
#include <stack>
#include <set>
#include <queue>
#include <cassert>

using namespace std;
using namespace chrono;

using Graph = map<int, vector<pair<int, double>>>; // Adjacency list representation
using Coordinates = map<int, pair<double, double>>; // Node coordinates

// Function prototypes
void parseGraph(const string& filename, Graph& graph, Coordinates& coordinates);
int findNearestNode(const Coordinates& coordinates, double lon, double lat);
double printPath(const map<int, int>& parent, int start, int goal, const Graph& graph);
double BFS(const Graph& graph, int start, int goal);
double DFS(const Graph& graph, int start, int goal);
double Dijkstra(const Graph& graph, int startNode, int targetNode);
double AStar(const Graph& graph, int startNode, int goalNode, const Coordinates& coords);

// Function to parse the graph from a file
void parseGraph(const string& filename, Graph& graph, Coordinates& coordinates) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    map<pair<double, double>, int> coordinateToId;
    int nodeId = 0;

    auto getId = [&](double lon, double lat) -> int {
        pair<double, double> coord = {lon, lat};
        auto it = coordinateToId.find(coord);
        if (it == coordinateToId.end()) {
            coordinateToId[coord] = nodeId;
            coordinates[nodeId] = coord;
            return nodeId++;
        }
        return it->second;
    };

    string line;
    while (getline(file, line)) {
        size_t colon = line.find(":");
        if (colon == string::npos) continue;

        string parentNode = line.substr(0, colon);
        string children = line.substr(colon + 1);

        double lon1, lat1;
        sscanf(parentNode.c_str(), "%lf,%lf", &lon1, &lat1);
        int parentId = getId(lon1, lat1);

        stringstream ss(children);
        string child;
        while (getline(ss, child, ';')) {
            double lon2, lat2, weight;
            if (sscanf(child.c_str(), "%lf,%lf,%lf", &lon2, &lat2, &weight) == 3) {
                int childId = getId(lon2, lat2);
                graph[parentId].emplace_back(childId, weight);
                graph[childId].emplace_back(parentId, weight);
            }
        }
    }
}

// Function to find the nearest node to a given coordinate
int findNearestNode(const Coordinates& coordinates, double lon, double lat) {
    int nearestNode = -1;
    double minDistance = numeric_limits<double>::infinity();

    for (const auto& [node, coord] : coordinates) {
        double distance = sqrt(pow(coord.first - lon, 2) + pow(coord.second - lat, 2));
        if (distance < minDistance) {
            minDistance = distance;
            nearestNode = node;
        }
    }
    return nearestNode;
}

// Function to print the path and calculate its total weight
double printPath(const map<int, int>& parent, int start, int goal, const Graph& graph) {
    vector<int> path;
    double weight = 0.0;

    for (int cur = goal; cur != start; cur = parent.at(cur)) {
        path.push_back(cur);
        for (const auto& [neighbor, w] : graph.at(parent.at(cur))) {
            if (neighbor == cur) weight += w;
        }
    }
    path.push_back(start);
    reverse(path.begin(), path.end());

    cout << "Final weight: " << weight << endl;
    return weight;
}

// BFS implementation
double BFS(const Graph& graph, int start, int goal) {
    auto start_time = high_resolution_clock::now();

    queue<int> q;
    set<int> visited;
    map<int, int> parent;
    q.push(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        if (current == goal) {
            auto end_time = high_resolution_clock::now();
            double totalWeight = printPath(parent, start, goal, graph);
            cout << "BFS time: " << duration_cast<milliseconds>(end_time - start_time).count() << " ms\n";
            return totalWeight;
        }

        for (const auto& [neighbor, _] : graph.at(current)) {
            if (!visited.count(neighbor)) {
                q.push(neighbor);
                parent[neighbor] = current;
            }
        }
    }
    cout << "BFS: Path not found.\n";
    return -1;
}

// DFS implementation
double DFS(const Graph& graph, int start, int goal) {
    auto start_time = high_resolution_clock::now();

    stack<int> s;
    set<int> visited;
    map<int, int> parent;
    s.push(start);

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        if (visited.count(current)) continue;
        visited.insert(current);

        if (current == goal) {
            auto end_time = high_resolution_clock::now();
            double totalWeight = printPath(parent, start, goal, graph);
            cout << "DFS time: " << duration_cast<milliseconds>(end_time - start_time).count() << " ms\n";
            return totalWeight;
        }

        for (const auto& [neighbor, _] : graph.at(current)) {
            if (!visited.count(neighbor)) {
                s.push(neighbor);
                parent[neighbor] = current;
            }
        }
    }
    cout << "DFS: Path not found.\n";
    return -1;
}

// Dijkstra's algorithm implementation
double Dijkstra(const Graph& graph, int startNode, int targetNode) {
    auto startTime = high_resolution_clock::now();

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> minHeap;
    map<int, double> shortestDistances;
    map<int, int> predecessors;

    for (const auto& node : graph) {
        shortestDistances[node.first] = numeric_limits<double>::infinity();
    }
    shortestDistances[startNode] = 0;

    minHeap.emplace(0, startNode);

    while (!minHeap.empty()) {
        auto [currentDistance, currentNode] = minHeap.top();
        minHeap.pop();

        if (currentNode == targetNode) {
            auto endTime = high_resolution_clock::now();
            double totalWeight = printPath(predecessors, startNode, targetNode, graph);
            cout << "Dijkstra time: "
                 << duration_cast<milliseconds>(endTime - startTime).count() << " ms\n";
            return totalWeight;
        }

        for (const auto& [neighbor, edgeWeight] : graph.at(currentNode)) {
            double potentialDistance = currentDistance + edgeWeight;

            if (potentialDistance < shortestDistances[neighbor]) {
                shortestDistances[neighbor] = potentialDistance;
                predecessors[neighbor] = currentNode;
                minHeap.emplace(potentialDistance, neighbor);
            }
        }
    }

    cout << "Dijkstra's algorithm: Path not found.\n";
    return -1;
}

// A* algorithm implementation
double AStar(const Graph& graph, int startNode, int goalNode, const Coordinates& coords) {
    auto startTime = high_resolution_clock::now();

    auto heuristicFunc = [&](int node) {
        auto [longitude1, latitude1] = coords.at(node);
        auto [longitude2, latitude2] = coords.at(goalNode);
        return sqrt(pow(longitude1 - longitude2, 2) + pow(latitude1 - latitude2, 2));
    };

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> openSet;
    map<int, double> gCosts;
    map<int, double> fCosts;
    map<int, int> cameFrom;

    for (const auto& node : graph) {
        gCosts[node.first] = numeric_limits<double>::infinity();
        fCosts[node.first] = numeric_limits<double>::infinity();
    }
    gCosts[startNode] = 0;
    fCosts[startNode] = heuristicFunc(startNode);

    openSet.emplace(fCosts[startNode], startNode);

    while (!openSet.empty()) {
        auto [currentFCost, currentNode] = openSet.top();
        openSet.pop();

        if (currentNode == goalNode) {
            auto endTime = high_resolution_clock::now();
            double totalWeight = printPath(cameFrom, startNode, goalNode, graph);
            cout << "A* execution time: " << duration_cast<milliseconds>(endTime - startTime).count() << " ms\n";
            return totalWeight;
        }

        for (const auto& [neighbor, edgeWeight] : graph.at(currentNode)) {
            double tentativeGCost = gCosts[currentNode] + edgeWeight;

            if (tentativeGCost < gCosts[neighbor]) {
                gCosts[neighbor] = tentativeGCost;
                fCosts[neighbor] = tentativeGCost + heuristicFunc(neighbor);
                cameFrom[neighbor] = currentNode;
                openSet.emplace(fCosts[neighbor], neighbor);
            }
        }
    }
    cout << "A*: Path not found.\n";
    return -1;
}

int main() {
    string filename = "spb_graph.txt";

    Graph graph;
    Coordinates coordinates;
    parseGraph(filename, graph, coordinates);

    double start_lon = 30.500172, start_lat = 59.936866; // Starting location   Belorusskaya street 6
    double goal_lon = 30.337795, goal_lat = 59.926835; // Goal location         Lomonosova street 9

    int start = findNearestNode(coordinates, start_lon, start_lat);
    int goal = findNearestNode(coordinates, goal_lon, goal_lat);

    if (start == -1 || goal == -1) {
        cerr << "Start or goal node not found by coordinates." << endl;
        return 1;
    }

    cout << "\nRunning BFS:\n";
    BFS(graph, start, goal);

    cout << "\nRunning DFS:\n";
    DFS(graph, start, goal);

    cout << "\nRunning Dijkstra:\n";
    Dijkstra(graph, start, goal);

    cout << "\nRunning A*:\n";
    AStar(graph, start, goal, coordinates);

    return 0;
}
