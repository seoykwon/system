TARGET=train
CC=gcc
CFLAGS=-Wall
OBJECTS=reserve.o departTime.o travelPath.o server.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

reserve.o: reserve.cc
	$(CC) $(CFLAGS) -c reserve.cc -o reserve.o

departTime.o: departTime.cc
	$(CC) $(CFLAGS) -c departTime.cc -o departTime.o

travelPath.o: travelPath.cc
	$(CC) $(CFLAGS) -c travelPath.cc -o travelPath.o

server.o: server.cc
	$(CC) $(CFLAGS) -c server.cc -o server.o

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)