arm: arm_clean arm_compiler arm_execute

local: clean compiler

compiler: main.o lib.o
	g++ -Os -o compiler lib.o main.o

main.o: test.c
	gcc -Os -c -o main.o test.c

lib.o: math03-4.cpp
	g++ -Os -c -o lib.o math03-4.cpp

arm_compiler: arm_main.o arm_lib.o
	arm-linux-gnueabi-g++ -Os -marm -o arm_compiler arm_lib.o arm_main.o

arm_main.o: test.c
	arm-linux-gnueabi-gcc -Os -marm -c -o arm_main.o test.c

arm_lib.o: math03-4.cpp
	arm-linux-gnueabi-g++ -Os -marm -c -o arm_lib.o math03-4.cpp

arm_execute: arm_compiler
	./arm_compiler < input.txt

clean:
	rm -f main.o lib.o

arm_clean:
	rm -f arm_main.o arm_lib.o
