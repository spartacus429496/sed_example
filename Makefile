all:
	gcc -o pskey_format pskey_format.c
	./pskey_format pb-207-csr8x11-rev4_gen1.txt
.PHONY:clean
clean:
	rm -f pskey_format PS_KEY_CSR8811_gen.txt
