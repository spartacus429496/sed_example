all:
	gcc -o a.out fopen_ex.c
	./a.out pb-207-csr8x11-rev4_gen1.txt
.PHONY:clean
clean:
	rm -f a.out
