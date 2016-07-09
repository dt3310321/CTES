
all:	txtdns-bad txtdns-ok

clean:	
	rm -f txtdns-bad txtdns-ok txtdns-bad.out txtdns-ok.out

txtdns-bad: txt-dns-file-bad.c my-util.c  
	$(CC) -o txtdns-bad txt-dns-file-bad.c my-util.c  -I . -lresolv

txtdns-ok: txt-dns-file-ok.c  my-util.c  
	$(CC) -o txtdns-ok txt-dns-file-ok.c my-util.c  -I . -lresolv
