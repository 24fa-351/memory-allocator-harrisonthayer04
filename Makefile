all: memtest_sys memtest_dev

memtest_sys: memtest.c
	gcc -o memtest_sys -DSYSTEM_MALLOC memtest.c
memtest_dev: memtest.c malloc.c heap.c malloc.h heap.h 
	gcc -o memtest_dev memtest.c malloc.c heap.c

clean:
	rm -f memtest_sys memtest_dev