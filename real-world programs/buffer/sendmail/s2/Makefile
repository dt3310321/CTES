
all:	ge-bad ge-ok

clean:	
	rm -f ge-bad ge-ok ge-bad.out ge-ok.out

ge-bad: main-bad.c  recipient-bad.c  util-bad.c
	$(CC) -o ge-bad main-bad.c  recipient-bad.c  util-bad.c -I .

ge-ok: main-ok.c  recipient-ok.c  util-ok.c
	$(CC) -o ge-ok main-ok.c  recipient-ok.c  util-ok.c -I .


