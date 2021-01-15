TESTGEN = testgen.c
SOURCE = math07-2.c
CHECK = check.c
FEATURE = feature.c
STDINPUT = input.bin
SIZE = 1000
BLOCK = 10000
# STDINPUT = > input.txt

test: gen run clean

test_win: gen run_win clean

check_test: gen run check clean

feature_test: run_feature clean

run_feature: build_feature
	./${FEATURE:.c=.exe}

build_feature:
	gcc ${FEATURE} -o ${FEATURE:.c=.exe}

build_src:
	gcc ${SOURCE} -o ${SOURCE:.c=.exe}

build_check:
	gcc ${CHECK} -DSIZE=${SIZE} -DBLOCK=${BLOCK} -o ${CHECK:.c=.exe}

gen:
	rm -rf ${STDINPUT}
	gcc -DEXTERNAL -DSIZE=${SIZE} -DBLOCK=${BLOCK} ${TESTGEN} -o ${TESTGEN:.c=.exe}
	./${TESTGEN:.c=.exe} ${STDINPUT} > start.txt

check: build_check
	./${CHECK:.c=.exe} ${STDINPUT} > end.txt

run: build_src
	./${SOURCE:.c=.exe} ${STDINPUT}
	# valgrind --tool=massif ./${SOURCE:.c=.exe} ${STDINPUT}

run_win:
	i686-w64-mingw32-gcc main.c -o main.exe
	WINEDEBUG=-all wine main.exe ${STDINPUT}

clean:
	rm -rf *.exe