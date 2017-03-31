all: p1 readme

p1: main.o scanner.o
	g++ -o testScanner -g main.o scanner.o

app.o: scanner.cpp
	g++ -c -g scanner.cpp

main.o: main.cpp
	g++ -c -g main.cpp


clean: remove clear success

remove:
	rm *.o p1

clear: 
	clear
	
success: 
	$(info SUCCESS)
	
readme:
	cat README
