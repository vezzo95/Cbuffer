CXXFLAGS = -DNDEBUG

main.exe: main.o person.o
	g++ main.o person.o -o main.exe

main.o: main.cpp
	g++ $(CXXFLAGS) -c main.cpp -o main.o

person.o: person.cpp
	g++ $(CXXFLAGS) -c person.cpp -o person.o

.PHONY: clean

clean:
	rm *.exe *.o
