
all:	ca-bad ca-ok

clean:	
	rm -f ca-bad ca-ok ca-bad.out ca-ok.out

ca-bad: crackaddr-bad.c
	$(CC) -o ca-bad crackaddr-bad.c

ca-ok: crackaddr-ok.c
	$(CC) -o ca-ok crackaddr-ok.c

