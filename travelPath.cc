#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include "departTime.h"

#define NUM_CITIES 26
#define NUM_PATHS 10
#define NUM_DAYS 31
#define NUM_DEPARTURE_TIMES 5

// Structure for a city
struct City {
    char name;
    int x;
    int y;
};

// Structure for an adjacency list node
struct AdjListNode {
    int dest;
    double distance;
    double travelTime;
    struct AdjListNode* next;
};

// Structure for an adjacency list
struct AdjList {
    struct AdjListNode* head;
    struct AdjListNode* cur; // Pointer to the current node
};

// Structure for a graph
struct Graph {
    int V;
    struct AdjList* array;
};

// Structure to store information about the shortest path
struct PathInfo {
    int distance;
    int previous[NUM_CITIES];
};

// Function prototypes
struct AdjListNode* newAdjListNode(int dest, double distance, double travelTime);
struct Graph* createGraph(int V);
void addEdge(struct Graph* graph, int src, int dest, double distance, double travelTime);
void printGraph(struct Graph* graph, struct City* cities);
void freeGraph(struct Graph* graph);
void generateRandomCityLocations(struct City* cities, int V);
double calculateDistance(double x1, double y1, double x2, double y2);
int pathExists(struct Graph* graph, int src, int dest);
void generateRandomPaths(struct Graph* graph, struct City* cities);

// Function to find the shortest path using Dijkstra's algorithm
struct PathInfo dijkstra(struct Graph* graph, int source) {
    struct PathInfo pathInfo;
    int distances[NUM_CITIES];
    int previous[NUM_CITIES];
    int visited[NUM_CITIES];

    // Initialize distances and visited arrays
    for (int i = 0; i < NUM_CITIES; i++) {
        distances[i] = INT_MAX;
        previous[i] = -1;
        visited[i] = 0;
    }

    distances[source] = 0;

    for (int i = 0; i < NUM_CITIES - 1; i++) {
        int minDistance = INT_MAX;
        int minIndex;

        for (int j = 0; j < NUM_CITIES; j++) {
            if (!visited[j] && distances[j] <= minDistance) {
                minDistance = distances[j];
                minIndex = j;
            }
        }

        int u = minIndex;
        visited[u] = 1;

        for (int v = 0; v < NUM_CITIES; v++) {
            int pathNumber = -1;
            for (struct AdjListNode* head = graph->array[u].head; head != NULL; head = head->next, pathNumber++) {
                if (head->dest == v) {
                    if (!visited[v] && distances[u] != INT_MAX && distances[u] + 1 < distances[v]) {
                        distances[v] = distances[u] + 1;
                        previous[v] = u;
                    }
                    break;
                }
            }
        }
    }

    pathInfo.distance = *distances;
    memcpy(pathInfo.previous, previous, sizeof(previous));

    return pathInfo;
}

// Function to find the shortest path from source to destination using Dijkstra's algorithm
int findShortestPath(struct Graph* graph, int source, int destination, int* path) {
    struct PathInfo pathInfo = dijkstra(graph, source);
    int currentIndex = destination;
    int pathIndex = 0;

    while (currentIndex != -1) {
        path[pathIndex++] = currentIndex;
        currentIndex = pathInfo.previous[currentIndex];
    }

    for (int i = 0; i < pathIndex / 2; i++) {
        int temp = path[i];
        path[i] = path[pathIndex - 1 - i];
        path[pathIndex - 1 - i] = temp;
    }

    return pathIndex;
}

// Function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, double distance, double travelTime) {
    struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->distance = distance;
    newNode->travelTime = travelTime;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph of V vertices
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList));
    for (int i = 0; i < V; ++i) {
        graph->array[i].head = NULL;
        graph->array[i].cur = NULL;
    }
    return graph;
}

// Function to add an edge to a directed graph
void addEdge(struct Graph* graph, int src, int dest, double distance, double travelTime) {
    struct AdjListNode* newNode = newAdjListNode(dest, distance, travelTime);
    if (graph->array[src].head == NULL) {
        graph->array[src].head = newNode;
        graph->array[src].cur = newNode;
    } else {
        graph->array[src].cur->next = newNode;
        graph->array[src].cur = newNode;
    }
}

// Function to print the adjacency list representation of graph
void printGraph(struct Graph* graph, struct City* cities) {
    for (int v = 0; v < graph->V; ++v) {
        struct AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of city %c\n head ", cities[v].name);
        while (pCrawl) {
            printf("-> %c", cities[pCrawl->dest].name);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

// Function to free the memory allocated for the graph
void freeGraph(struct Graph* graph) {
    for (int v = 0; v < graph->V; ++v) {
        struct AdjListNode* pCrawl = graph->array[v].head;
        while (pCrawl) {
            struct AdjListNode* temp = pCrawl;
            pCrawl = pCrawl->next;
            free(temp);
        }
    }
    free(graph->array);
    free(graph);
}

// Function to generate random coordinates for cities
void generateRandomCityLocations(struct City* cities, int V) {
    for (int i = 0; i < V; ++i) {
        cities[i].name = 'a' + i; // Assign city names from A to Z
        cities[i].x = (rand() % 2001) - 1000; // x in range -1000 to 1000
        cities[i].y = (rand() % 2001) - 1000; // y in range -1000 to 1000
    }
}

// Function to calculate distance between two cities
double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Function to check if a path exists
int pathExists(struct Graph* graph, int src, int dest) {
    struct AdjListNode* pCrawl = graph->array[src].head;
    while (pCrawl) {
        if (pCrawl->dest == dest) {
            return 1;
        }
        pCrawl = pCrawl->next;
    }
    return 0;
}

// Function to generate random paths ensuring no bidirectional paths
void generateRandomPaths(struct Graph* graph, struct City* cities) {
    for (int i = 0; i < graph->V; ++i) {
        int generatedPaths = 0;
        while (generatedPaths < NUM_PATHS) {
            int dest = rand() % graph->V;
            if (dest != i && !pathExists(graph, i, dest) && !pathExists(graph, dest, i)) {
                double distance = calculateDistance(cities[i].x, cities[i].y, cities[dest].x, cities[dest].y);
                double travelTime = distance / 200.0; // Assuming an average speed of 200 km/h
                addEdge(graph, i, dest, distance, travelTime);
                generatedPaths++;
            }
        }
    }
}

struct DepartureTime findFastestDeparture(struct DepartureTime departureTable[NUM_DAYS][NUM_CITIES][NUM_PATHS][NUM_DEPARTURE_TIMES], int startDay, int city, int path, int expectedTime) {
    struct DepartureTime fastestDeparture = { .hour = 24, .minute = 60, .day = -1 };

    for (int d = startDay; d < NUM_DAYS; ++d) {
        for (int time = 0; time < NUM_DEPARTURE_TIMES; ++time) {
            int departureTime = departureTable[d][city][path][time].hour * 60 + departureTable[d][city][path][time].minute;
            if ((departureTime >= expectedTime && d == startDay) || d > startDay) {
                if (fastestDeparture.day == -1 || departureTime < (fastestDeparture.hour * 60 + fastestDeparture.minute)) {
                    fastestDeparture = departureTable[d][city][path][time];
                    fastestDeparture.day = d + 1;
                }
            }
        }
    }
    return fastestDeparture;
}


// // Driver program to test above functions
// int main() {
//     srand(time(NULL)); // Seed the random number generator

//     // Create the graph and cities
//     struct Graph* graph = createGraph(NUM_CITIES);
//     struct City cities[NUM_CITIES];

//     // Initialize city names
//     for (int i = 0; i < NUM_CITIES; ++i) {
//         cities[i].name = 'A' + i;
//     }

//     // Generate random city locations
//     generateRandomCityLocations(cities, NUM_CITIES);

//     // Generate random paths
//     generateRandomPaths(graph, cities);

//     // Create 4D array for departure time table
//     struct DepartureTime departureTable[NUM_DAYS][NUM_CITIES][NUM_PATHS][NUM_DEPARTURE_TIMES];
//     printGraph(graph, cities);
//     // Generate departure time table for one month
//     generateDepartureTimeTable(departureTable);

//     // Print departure time table for one month
//     // printDepartureTimeTable(departureTable);

//     // Free the memory allocated for the graph
//     freeGraph(graph);

//     return 0;
// }
