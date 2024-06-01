#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_CITIES 26
#define NUM_PATHS 10
#define NUM_DAYS 31
#define NUM_DEPARTURE_TIMES 5

// Structure for departure time
struct DepartureTime {
    int day;
    int hour;
    int minute;
    int nextCity;
};

// Function to generate random departure times for all cities and paths for one month
void generateDepartureTimeTable(struct DepartureTime departureTable[NUM_DAYS][NUM_CITIES][NUM_PATHS][NUM_DEPARTURE_TIMES]) {
    for (int day = 0; day < NUM_DAYS; ++day) {
        for (int city = 0; city < NUM_CITIES; ++city) {
            for (int path = 0; path < NUM_PATHS; ++path) {
                for (int time = 0; time < NUM_DEPARTURE_TIMES; ++time) {
                    departureTable[day][city][path][time].day = day + 1; // Day of the month
                    departureTable[day][city][path][time].hour = rand() % 24; // Random hour between 0 and 23
                    departureTable[day][city][path][time].minute = rand() % 60; // Random minute between 0 and 59
                }

                // Sort the departure times for the current day, city, and path
                for (int i = 0; i < NUM_DEPARTURE_TIMES - 1; ++i) {
                    for (int j = 0; j < NUM_DEPARTURE_TIMES - i - 1; ++j) {
                        int time1 = departureTable[day][city][path][j].hour * 60 + departureTable[day][city][path][j].minute;
                        int time2 = departureTable[day][city][path][j + 1].hour * 60 + departureTable[day][city][path][j + 1].minute;
                        if (time1 > time2) {
                            struct DepartureTime temp = departureTable[day][city][path][j];
                            departureTable[day][city][path][j] = departureTable[day][city][path][j + 1];
                            departureTable[day][city][path][j + 1] = temp;
                        }
                    }
                }
            }
        }
    }
}

// Function to print departure time table for one month
void printDepartureTimeTable(struct DepartureTime departureTable[NUM_DAYS][NUM_CITIES][NUM_PATHS][NUM_DEPARTURE_TIMES]) {
    for (int day = 0; day < NUM_DAYS; ++day) {
        printf("Day %d:\n", day + 1);
        for (int city = 0; city < NUM_CITIES; ++city) {
            printf("City %c:\n", 'A' + city);
            for (int path = 0; path < NUM_PATHS; ++path) {
                printf("  Path %d:\n", path + 1);
                for (int time = 0; time < NUM_DEPARTURE_TIMES; ++time) {
                    printf("    Departure Time: %02d:%02d\n", 
                           departureTable[day][city][path][time].hour,
                           departureTable[day][city][path][time].minute);
                }
            }
        }
        printf("\n");
    }
}

