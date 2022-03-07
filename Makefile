hts221_x86: main.o hts221.o
	gcc main.o hts221.o -o hts221_x86

hts221_arm: main.o hts221.o
	aarch64-linux-gnu-gcc main.o hts221.o -o hts221_arm

main.o: main.c
	gcc -c main.c

hts221.o: hts221.c hts221.h
	gcc -c hts221.c

clean:
	rm *.o
