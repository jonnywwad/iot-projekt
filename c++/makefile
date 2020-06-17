

prog: main.o mqtt.o
	g++ mqtt.o main.o -lpaho-mqtt3c -o prog

main.o: main.cpp
	g++ -c -o main.o main.cpp 

mqtt.o: mqtt.c
	gcc -c -o mqtt.o mqtt.c



#
#==alles in C (auch main.c)==
#prog: main.o mqtt.o
	#gcc -o prog main.o mqtt.o -lpaho-mqtt3c
#
#main.o: main.c
	#gcc -c main.c
#
#mqtt.o: mqtt.c
	#gcc -c mqtt.c


clean:
	rm -f *.o prog
