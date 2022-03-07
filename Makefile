hts221_arm: main.c hts221.c
	aarch64-linux-gnu-gcc main.c hts221.c -o hts221_arm
	gcc main.c hts221.c -o hts221_x86

clean:
	rm *.o
