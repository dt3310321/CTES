
all:	sig-bad sig-ok create

clean:	
	rm -f sig-bad sig-ok create sig-bad.out sig-ok.out SIGFILE

sig-bad: sig-bad.c
	$(CC) -o sig-bad sig-bad.c -I . -lresolv

sig-ok: sig-ok.c
	$(CC) -o sig-ok sig-ok.c -I . -lresolv

create:	create_msg_file.c
	$(CC) -o create create_msg_file.c -lresolv ; \
	./create
	



