test: compile execute
	od -vx result_test.bmp > test.txt
	od -vx result_desired.bmp > correct.txt
	diff -y --suppress-common-lines test.txt correct.txt | wc -l
	diff -y --suppress-common-lines test.txt correct.txt > difference.txt

compile:
	gcc -m64 -mavx main.c -o main.o

execute: compile
	./main.o dest_test.bmp source_test.bmp result_test.bmp