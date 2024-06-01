#ifndef TRAVELPATH_H
#define TRAVELPATH_H

struct AdjListNode {
    int dest;
    double distance;
    double travelTime;
    struct AdjListNode* next;
};

struct AdjList {
    struct AdjListNode* head;
    struct AdjListNode* cur;
};

struct Graph {
    int V;
    struct AdjList* array;
};

struct City {
    char name;
    double x;
    double y;
};

// Structure to store information about the shortest path
struct PathInfo {
    int distance;
    int previous[NUM_CITIES];
};

struct AdjListNode* newAdjListNode(int dest, double distance, double travelTime);
struct Graph* createGraph(int V);
void addEdge(struct Graph* graph, int src, int dest, double distance, double travelTime);
void printGraph(struct Graph* graph, struct City* cities);
void freeGraph(struct Graph* graph);
void generateRandomCityLocations(struct City* cities, int V);
double calculateDistance(double x1, double y1, double x2, double y2);
int pathExists(struct Graph* graph, int src, int dest);
void generateRandomPaths(struct Graph* graph, struct City* cities);
struct PathInfo dijkstra(struct Graph* graph, int source);
int findShortestPath(struct Graph* graph, int source, int destination, int* path);

#endif // TRAVELPATH_H
