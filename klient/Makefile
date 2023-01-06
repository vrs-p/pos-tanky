LIBS=-L src/lib -lsfml-system -lsfml-network -lsfml-graphics -lsfml-window -pthread
INCLUDE=-I src/include

all: client

Button.o: windows/elements/Button.h windows/elements/Button.cpp
	g++ $(INCLUDE) -c "windows/elements/Button.cpp" -o Button.o

Textbox.o: windows/elements/Textbox.h windows/elements/Textbox.cpp
	g++ $(INCLUDE) -c "windows/elements/Textbox.cpp" -o Textbox.o

Menu.o: windows/Menu.h windows/Menu.cpp
	g++ $(INCLUDE) -c "windows/Menu.cpp" -o Menu.o

ShowScore.o: windows/ShowScore.h windows/ShowScore.cpp
	g++ $(INCLUDE) -c "windows/ShowScore.cpp" -o ShowScore.o

Bullet.o: bullet/Bullet.h bullet/Bullet.cpp
	g++ $(INCLUDE) -c "bullet/Bullet.cpp" -o Bullet.o

Tank.o: tank/Tank.h tank/Tank.cpp
	g++ $(INCLUDE) -c "tank/Tank.cpp" -o Tank.o

Application.o: application/Application.h application/Application.cpp
	g++ $(INCLUDE) -c "application/Application.cpp" -o Application.o

main.o: main.cpp
	g++ $(INCLUDE) -c "main.cpp" -o main.o

client: main.o Button.o Textbox.o Menu.o ShowScore.o Bullet.o Tank.o Application.o
	@echo "** Building game"
	g++ -o client Button.o Textbox.o Menu.o ShowScore.o Bullet.o Tank.o Application.o main.o $(INCLUDE) $(LIBS)

clean:
	@echo "** Removing object files and executable..."
	rm -f client *.o