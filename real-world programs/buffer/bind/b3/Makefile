
all:	iquery-bad iquery-ok  create

clean:	
	rm -f iquery-bad iquery-ok create iquery-bad.out iquery-ok.out

iquery-bad: iquery-bad.c
	$(CC) -o iquery-bad iquery-bad.c -lresolv

iquery-ok: iquery-ok.c
	$(CC) -o iquery-ok iquery-ok.c -lresolv

create: create_iquery.c
	$(CC) -o create create_iquery.c  -lresolv ; \
	./create


