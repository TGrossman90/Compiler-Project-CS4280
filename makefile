all: p3 readme

p3: main.o scanner.o parser.o treePrint.o semantics.o stack.o
	g++ -o testSem -g main.o scanner.o parser.o treePrint.o semantics.o stack.o
	
stack.o: stack.cpp
	g++ -c -g stack.cpp
	
semantics.o: semantics.cpp
	g++ -c -g semantics.cpp

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
	rm *.o testSem

clear: 
	clear
	
success: 
	$(info SUCCESS)
	
readme:
	cat README