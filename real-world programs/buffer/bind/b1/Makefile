
all:	nxt-bad nxt-ok

clean:	
	rm -f nxt-bad nxt-ok nxt-bad.out nxt-ok.out

nxt-bad: nxt-bad.c 
	$(CC) -o nxt-bad nxt-bad.c -I .  -lresolv

nxt-ok: nxt-ok.c 
	$(CC) -o nxt-ok nxt-ok.c -I .  -lresolv
