
all:	m2-bad m2-ok

clean:	
	rm -f m2-bad m2-ok m2-bad.out m2-ok.out

m2-bad: mime2-bad.c
	$(CC) -o m2-bad mime2-bad.c  -I .

m2-ok: mime2-ok.c
	$(CC) -o m2-ok mime2-ok.c  -I .

