all: p2 readme

p2: main.o scanner.o parser.o treePrint.o
	g++ -o testFrontEnd -g main.o scanner.o parser.o treePrint.o

parser.o: parser.cpp
	g++ -c -g parser.cpp
	
treePrint.o: treePrint.cpp
	g++ -c -g treePrint.cpp
	
scanner.o: scanner.cpp
	g++ -c -g scanner.cpp

main.o: main.cpp
	g++ -c -g main.cpp

clean: remove clear success

remove:
	rm *.o testFrontEnd

clear: 
	clear
	
success: 
	$(info SUCCESS)
	
readme:
	cat README