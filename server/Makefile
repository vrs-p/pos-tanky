LIBS=-lsfml-network -pthread

all: server

main.o: main.cpp
	g++ -c "main.cpp" -o main.o

Client.o: client/Client.h client/Client.cpp
	g++ -c "client/Client.cpp" -o Client.o

Application.o: application/Application.h application/Application.cpp
	g++ -c "application/Application.cpp" -o Application.o

server: main.o Client.o Application.o
	@echo "** Building game"
	g++ -o server main.o Client.o Application.o $(LIBS)

clean:
	@echo "** Removing object files and executable..."
	rm -f server *.o