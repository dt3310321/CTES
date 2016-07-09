
all:	prescan-bad prescan-ok

clean:	
	rm -f prescan-bad prescan-ok prescan-bad.out prescan-ok.out

prescan-bad: prescan-overflow-bad.c
	$(CC) -o prescan-bad prescan-overflow-bad.c

prescan-ok: prescan-overflow-ok.c
	$(CC) -o prescan-ok prescan-overflow-ok.c


