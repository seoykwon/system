#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "reserve.h"
#include "departTime.h"
#include "travelPath.h"

// Function to trim leading and trailing whitespace
void trim(char *str) {
    char *start = str;
    char *end = str + strlen(str) - 1;

    // Trim leading space
    while (isspace(*start)) {
        start++;
    }

    // Trim trailing space
    while (end > start && isspace(*end)) {
        end--;
    }

    // Move the trimmed string to the beginning of the input buffer
    memmove(str, start, end - start + 1);
    str[end - start + 1] = '\0'; // Null-terminate the string
}

int findPathNumber(struct Graph* graph, char source, char destination) {
    struct AdjListNode* head = graph->array[source - 'a'].head;
    int path = 0;
    while (head != NULL) {
        if (head->dest == destination - 'a') {
            return path;
        }
        head = head->next;
        path++;
    }
    return -1; // Path not found
}

struct DepartureTime findFastestDeparture(struct DepartureTime departureTable[NUM_DAYS][NUM_CITIES][NUM_PATHS][NUM_DEPARTURE_TIMES], int day, int city, int path, int expectedTime, int nextCity) {
    struct DepartureTime fastestDeparture = { .hour = 24, .minute = 60, .day = -1 };
    
    for (int d = day; d < NUM_DAYS; ++d) {
        for (int time = 0; time < NUM_DEPARTURE_TIMES; ++time) {
            int departureTime = departureTable[d][city][path][time].hour * 60 + departureTable[d][city][path][time].minute;
            if ((departureTime >= expectedTime && d == day) || d > day) {
                if (fastestDeparture.day == -1 || departureTime < (fastestDeparture.hour * 60 + fastestDeparture.minute)) {
                    fastestDeparture = departureTable[d][city][path][time];
                    fastestDeparture.day = d + 1;
                    if (nextCity != -1) {
                        fastestDeparture.nextCity = nextCity;
                    }
                }
            }
        }
    }
    return fastestDeparture;
}

void to24HourFormat(const char* timeStr, int* hour, int* minute) {
    char period[3];
    sscanf(timeStr, "%d:%d %2s", hour, minute, period);
    if (strcasecmp(period, "pm") == 0 && *hour != 12) {
        *hour += 12;
    } else if (strcasecmp(period, "am") == 0 && *hour == 12) {
        *hour = 0;
    }
}

void to12HourFormat(int hour, int minute, char* timeStr) {
    char period[3] = "am";
    if (hour >= 12) {
        strcpy(period, "pm");
        if (hour > 12) hour -= 12;
    } else if (hour == 0) {
        hour = 12;
    }
    snprintf(timeStr, 10, "%d:%02d%s", hour, minute, period);
}

int main() {
    srand(time(0));
    int V = 26;
    struct Graph* graph = createGraph(V);
    struct City cities[V];

    // Initialize city names
    for (int i = 0; i < V; ++i) {
        cities[i].name = 'a' + i;
    }

    // Generate random city locations
    generateRandomCityLocations(cities, V);

    // Generate random paths
    generateRandomPaths(graph, cities);
    printGraph(graph, cities);

    struct DepartureTime departureTable[NUM_DAYS][NUM_CITIES][NUM_PATHS][NUM_DEPARTURE_TIMES];
    generateDepartureTimeTable(departureTable);

    RBTree* tree = createRBTree();

    char input[100];
    char userName[50], source[50], destination[50], departure[50];

    // Prompt user for input
    printf("Book a ticket (format: name, source, destination, departureTime-date(9:00am-5)): ");
    fgets(input, sizeof(input), stdin);

    // Remove the newline character from the end of the input if it exists
    input[strcspn(input, "\n")] = 0;

    // Split the input string by commas and assign each part to the respective variable
    char *token = strtok(input, ",");
    if (token != NULL) strcpy(userName, token);

    token = strtok(NULL, ",");
    if (token != NULL) strcpy(source, token);

    token = strtok(NULL, ",");
    if (token != NULL) strcpy(destination, token);

    token = strtok(NULL, ",");
    if (token != NULL) strcpy(departure, token);

    trim(userName);
    trim(source);
    trim(destination);
    trim(departure);

    // Print the values to check if they are read correctly
    printf("%s %s %s %s\n", userName, source, destination, departure);

    // Convert source and destination to indices
    int sourceIndex = tolower(source[0]) - 'a';
    int destinationIndex = tolower(destination[0]) - 'a';

    int day = atoi(strchr(departure, '-') + 1); // Extracting the date
    char timeStr[10];
    strncpy(timeStr, departure, strchr(departure, '-') - departure);
    timeStr[strchr(departure, '-') - departure] = '\0';

    int hour, minute;
    to24HourFormat(timeStr, &hour, &minute);
    int expectedTime = hour * 60 + minute;

    // Find the shortest path
    int path[NUM_CITIES];
    int pathLength = findShortestPath(graph, sourceIndex, destinationIndex, path);

    if (pathLength == 0) {
        printf("No path from %s to %s.\n", source, destination);
        freeGraph(graph);
        return -1;
    }

    // Find the fastest departure times for each leg of the journey
    struct DepartureTime fastestDeparture[NUM_CITIES];
    int currentDay = day - 1;
    int currentTime = expectedTime;

    for (int i = 0; i < pathLength - 1; i++) {
        int currentCity = path[i];
        int nextCity = path[i + 1];
        int pathNumber = 0;
        for (struct AdjListNode* head = graph->array[currentCity].head; head != NULL; head = head->next, pathNumber++) {
            if (head->dest == nextCity) {
                fastestDeparture[i] = findFastestDeparture(departureTable, currentDay, currentCity, pathNumber, currentTime, nextCity);
                currentDay = fastestDeparture[i].day - 1;
                currentTime = fastestDeparture[i].hour * 60 + fastestDeparture[i].minute;
                break;
            }
        }
    }

    // Print the fastest departure times for each leg
    for (int i = 0; i < pathLength - 1; i++) {
        char fastestTimeStr[10];
        to12HourFormat(fastestDeparture[i].hour, fastestDeparture[i].minute, fastestTimeStr);
        printf("Fastest departure from %c to %c on day %d at %s\n", 'a' + path[i], 'a' + path[i + 1], fastestDeparture[i].day, fastestTimeStr);
    }

    // Make reservations with the fastest departure times
    for (int i = 0; i < pathLength - 1; i++) {
        char fastestTimeStr[10];
        to12HourFormat(fastestDeparture[i].hour, fastestDeparture[i].minute, fastestTimeStr);
        makeReservation(tree, userName, 'a' + path[i], 'a' + path[i + 1], fastestTimeStr, fastestDeparture[i].day);
    }

    // Print the Red-Black Tree to see the reservations
    rbPrint(tree->root, tree->nil, 1);

    // Free the memory
    freeGraph(graph);
    return 0;
}
