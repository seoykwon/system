#ifndef DEPARTTIME_H
#define DEPARTTIME_H

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

void generateDepartureTimeTable(struct DepartureTime departureTable[NUM_DAYS][NUM_CITIES][NUM_PATHS][NUM_DEPARTURE_TIMES]);
void printDepartureTimeTable(struct DepartureTime departureTable[NUM_DAYS][NUM_CITIES][NUM_PATHS][NUM_DEPARTURE_TIMES]);

#endif // DEPARTTIME_H
