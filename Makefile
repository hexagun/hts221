hts221: main.o hts221.o
	gcc main.o hts221.o -o hts221

main.o: main.c
	gcc -c main.c

hts221.o: hts221.c hts221.h
	gcc -c hts221.c

clean:
	rm *.o
