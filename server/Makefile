LIBS=-lsfml-network -pthread

all: server

main.o: main.cpp
	g++ -c "main.cpp" -o main.o

Client.o: Client.h Client.cpp
	g++ -c "Client.cpp" -o Client.o

Application.o: Application.h Application.cpp
	g++ -c "Application.cpp" -o Application.o

server: main.o Client.o Application.o
	@echo "** Building game"
	g++ -o server main.o Client.o Application.o $(LIBS)

clean:
	@echo "** Removing object files and executable..."
	rm -f server *.o